"""
three_in_a_row_solver.py

Features:
- Calibration hotkeys:
  F1: sample colors (press three times: black, white, empty)
  F2: mark top-left diagonal tile (start)
  F3: mark another diagonal tile (press multiple times for each diagonal tile)
  F4: mark final diagonal tile (bottom-right)
  F5: toggle overlay showing detected tiles and states - actually a nice change would be to show all the places it wants to click next (display only what's in the queue to be clicked)
        * apply_moves_and_clicks is where the update for each move is made, which is where the program should update the overlay's remaining click list (also the dots should be matching black or white for what will be placed)
  F8: start/stop solver run (toggle)
  F9: kill program (exit)

- Solves puzzle by scanning screen tiles, applying logic rules (2-in-row, 1-gap,
  half-satisfied line, one-off combinational check), and clicking:
    left-click = black, right-click = white

- Overlay drawn with tkinter canvas (best-effort transparent window).

next plans: battleship, nonogrid, tents, tracks?
or make a prolog solver for 3iar, nonogrid, tents, slants, whatever and just have a python scanner
"""

import threading
import time
import math
from collections import defaultdict
from itertools import combinations
from copy import deepcopy

from pynput import mouse, keyboard
from PIL import ImageGrab, Image, ImageStat, ImageFilter, ImageOps, ImageDraw, ImageFont

# ---------------------------
# Configuration / Globals
# ---------------------------
# You can tweak these:
SAMPLE_RADIUS = 1             # radius (px) for color sample around mouse when calibrating
SCAN_SAMPLE_RADIUS = 2        # sample area when scanning tiles
COLOR_MATCH_THRESHOLD = 60    # max euclidean distance to accept a color match
CLICK_DELAY = 0.00001            # delay between mouse clicks in seconds (0.00001 anything lower than 0.00001 really has no effect) 0.05
OVERLAY_ALPHA = 0.8           # overlay opacity (if supported)
REFRESH_DELAY = 0.5           # delay after F8 to allow screen to refresh before scanning
INTERP_STEP = 0           #0.001 step time for interpolated mouse movement (set to 0 or negative for instant). If both INTERP_STEP and INTERP_VELOCITY are set, INTERP_STEP takes priority
INTERP_VELOCITY = 10      # 10 pixels per second for interpolated mouse movement (set to 0 or negative for instant). Will not run if INTERP_STEP>0

# Program state (protected by lock where needed)
state = {
    "diag_points": [],       # points recorded by F2, F3*(many), F4
    "top_left": None,        # will be diag_points[0]
    "bottom_right": None,    # last diag point
    "n": None,               # board size (in tiles)
    "tile_dx": None,         # delta x between adjacent tiles along diagonal (per step)
    "tile_dy": None,         # delta y between adjacent tiles along diagonal (per step)
    "color_samples": {"black": None, "white": None, "empty": None},
    "overlay_on": True,
    "running": False,
    "killed": False,
    "board_centers": [],     # list of (x,y) centers row-major
    "last_scan": None,       # last scanned board array
    "board" : None,            # current board state in memory
    "autoplay": False
}

lock = threading.RLock()

# For mouse control:
mouse_controller = mouse.Controller()
keyboard_controller = keyboard.Controller()

# For listeners:
listener_threads = []

# For overlay:
overlay_root = None
overlay_canvas = None
overlay_visible = False # Will also control board state output to console

# ---------------------------
# Utility functions
# ---------------------------
def distance(c1, c2):
    return math.sqrt(sum((a - b) ** 2 for a, b in zip(c1, c2)))

def sample_color_at(pos, radius=SAMPLE_RADIUS):
    """
    Take a small square screenshot around pos and return average RGB tuple.
    pos: (x,y)
    """
    x, y = int(pos[0]), int(pos[1])
    left, top = x - radius, y - radius
    right, bottom = x + radius, y + radius
    img = ImageGrab.grab(bbox=(left, top, right + 1, bottom + 1))
    # convert to RGB and compute mean:
    stat = ImageStat.Stat(img.convert("RGB"))
    r, g, b = stat.mean
    return (int(r), int(g), int(b))

def nearest_color_label(sample_rgb):
    """
    Compare sample_rgb to calibrated black/white/empty and return 'black','white','empty' or None
    """
    with lock:
        samples = state["color_samples"]
    distances = {}
    for k, v in samples.items():
        if v is None:
            distances[k] = float("inf")
        else:
            distances[k] = distance(sample_rgb, v)
    # pick nearest if below threshold
    winner = min(distances, key=distances.get)
    if distances[winner] <= COLOR_MATCH_THRESHOLD:
        return winner
    return None

# ---------------------------
# Calibration handlers (hotkeys)
# ---------------------------
def record_diagonal_point(pos):
    with lock:
        state["diag_points"].append(pos)
    print(f"[CALIB] recorded diagonal point {pos}  (total: {len(state['diag_points'])})")

def finalize_diagonal():
    """
    Called when F3 pressed to finish diagonal calibration.
    Use recorded diag_points to compute n and tile deltas.
    Assumptions:
      - User pressed F1 for top-left, then F2 many times along the negative diagonal, then F3 for last.
      - The recorded points are centers for tiles along that diagonal in order.
    """
    with lock:
        pts = list(state["diag_points"])
    if len(pts) < 2:
        print("[CALIB] Need at least 2 diagonal points (start and end).")
        return
    top_left = pts[0]
    bottom_right = pts[-1]
    n = len(pts)  # number of tiles along diagonal
    dx = (bottom_right[0] - top_left[0]) / (n - 1)
    dy = (bottom_right[1] - top_left[1]) / (n - 1)
    # compute board centers row-major (r,c)
    centers = []
    for r in range(n):
        row = []
        for c in range(n):
            cx = top_left[0] + c * dx
            cy = top_left[1] + r * dy
            row.append((int(round(cx)), int(round(cy))))
        centers.append(row)
    with lock:
        state.update({
            "top_left": top_left,
            "bottom_right": bottom_right,
            "n": n,
            "tile_dx": dx,
            "tile_dy": dy,
            "board_centers": centers
        })
    print(f"[CALIB] Board calibrated: size {n}x{n}, tile delta ≈ ({dx:.2f},{dy:.2f})")

def sample_color_sequence(sample_pos):
    """
    Called when F4 is pressed. The user should press F4 three times: black, white, empty (in that order).
    This function stores the samples in order into the color_samples dict.
    """
    rgb = sample_color_at(sample_pos, radius=SAMPLE_RADIUS)
    with lock:
        cs = state["color_samples"]
        if cs["black"] is None:
            cs["black"] = rgb
            tag = "black"
        elif cs["white"] is None:
            cs["white"] = rgb
            tag = "white"
        elif cs["empty"] is None:
            cs["empty"] = rgb
            tag = "empty"
        else:
            # overwrite empty if all already set
            cs["empty"] = rgb
            tag = "empty(overwrite)"
    print(f"[CALIB] Sampled {tag} color = {rgb}")

# ---------------------------
# Screen scanning and clicking
# ---------------------------
# this can definitely be made faster by sampling the entire board as one screenshot, then analyzing coordinates within the screenshot instead of doing many small screenshots.
# Screenshots take the longest time per operation, so taking one screenshot of the whole board and then cropping it for each tile would be faster.
# new approach: screenshot from upper left coordinate to bottom right coordinate. using the normal tile spacing extrapolation, sample each tile center from that single screenshot.
# oh my goodness its so much faster now
def scan_board():
    print("[SCAN] Scanning board...")
    """
    Sample each tile center and return board as 2D array of values:
      -1 -> black
       1 -> white
       0 -> empty
    If a tile color cannot be confidently matched, value is None.
    """
    board = []
    board_screenshot = ImageGrab.grab(bbox=(state["diag_points"][0][0]-(state["tile_dx"]/2), state["diag_points"][0][1]-(state["tile_dy"]/2), state["diag_points"][-1][0]+(state["tile_dx"]/2), state["diag_points"][-1][1]+(state["tile_dy"]/2)))
    # save board screenshot for debugging
    #board_screenshot.save("board_scan_debug.png")
    # now using the board_screenshot we can sample each tile center using tile_dx and tile_dy
    with lock:
        n = state["n"]
        dx = state["tile_dx"]
        dy = state["tile_dy"]
        top_left = state["top_left"]
        centers = state["board_centers"]
    if n is None or dx is None or dy is None or top_left is None or centers is None:
        print("[SCAN] Board not calibrated yet.")
        return None
    for r in range(n):
        row = []
        for c in range(n):
            # sample colors at (cx,cy) in the screenshot
            # centers start at +dx/2,+dy/2 offset in the screenshot from top left
            rel_cx = int((state["tile_dx"]/2)+(c*state["tile_dx"]))
            rel_cy = int((state["tile_dy"]/2)+(r*state["tile_dy"]))
            # pick the area around (rel_cx, rel_cy) in the screenshot
            left = int(round(rel_cx - SCAN_SAMPLE_RADIUS))
            top = int(round(rel_cy - SCAN_SAMPLE_RADIUS))
            right = int(round(rel_cx + SCAN_SAMPLE_RADIUS + 1))
            bottom = int(round(rel_cy + SCAN_SAMPLE_RADIUS + 1))
            tile_img = board_screenshot.crop((left, top, right, bottom))
            stat = ImageStat.Stat(tile_img.convert("RGB"))
            sample_rgb = tuple(int(v) for v in stat.mean)
            if overlay_visible:
                print(f"[SCAN] Sampled RGB color at ({rel_cx}, {rel_cy}): {sample_rgb}")
            label = nearest_color_label(sample_rgb)
            if label == "black":
                val = -1
            elif label == "white":
                val = 1
            elif label == "empty":
                val = 0
            else:
                val = None
            row.append(val)
            if overlay_visible:
                print(f"[SCAN] Tile ({r},{c}) at screen ({rel_cx},{rel_cy}) sampled color {sample_rgb} labeled as {label} -> {val}")
        board.append(row)
    with lock:
        state["last_scan"] = deepcopy(board)
    
    print("[SCAN] Board scan complete")
    if overlay_visible:
        print_colored_board(board)
    state["board"] = deepcopy(board)
    return board

def click_tile(r, c, color_value):
    """
    Click the center of tile (r,c). color_value: -1 black (left), 1 white (right)
    Adds a small delay and returns True if click was performed.
    """
    with lock:
        centers = state["board_centers"]
    cx, cy = centers[r][c]
    # Move mouse for click delay time then click:
    # a cool different interpolation could be a max velocity so instead of the same time for a distance with a velocity different for each move, its the same velocity between all moves with a different time for each move. could be more satisfying to watch small clusters and large apart moves
    if(INTERP_STEP>0):
        # amount of steps = click_delay/interp_step
        steps = int(CLICK_DELAY / INTERP_STEP)
        # move 1/steps of the way from start to (cx, cy) every interp_step seconds
        start_pos = get_mouse_position()
        for i in range(1, steps + 1):
            new_x = start_pos[0] + ((cx - start_pos[0]) * (i / steps))
            new_y = start_pos[1] + ((cy - start_pos[1]) * (i / steps))
            mouse_controller.position = (new_x/2, new_y/2)
            time.sleep(INTERP_STEP)            
        mouse_controller.position = (cx/2, cy/2) # ensure final position is exact, even if there is lag or some other reason it didnt make it to the final position
    elif(INTERP_VELOCITY>0):
        # move at a constant velocity
        start_pos = get_mouse_position()
        dist = distance(start_pos, (cx, cy))
        if dist == 0:
            steps = 1
            step_time = 0
        else:
            steps = int(dist / INTERP_VELOCITY)
            step_time = CLICK_DELAY / steps
        for i in range(1, steps + 1):
            new_x = start_pos[0] + ((cx - start_pos[0]) * (i / steps))
            new_y = start_pos[1] + ((cy - start_pos[1]) * (i / steps))
            mouse_controller.position = (new_x/2, new_y/2)
            time.sleep(step_time)
        mouse_controller.position = (cx/2, cy/2) # ensure final position is exact
    else: # instant move if INTERP_STEP is 0 or negative
        mouse_controller.position = (cx/2, cy/2)
        time.sleep(CLICK_DELAY)
    if color_value == -1:
        mouse_controller.click(mouse.Button.left)
    elif color_value == 1:
        mouse_controller.click(mouse.Button.right)
    else:
        print(f"[CLICK] Invalid color_value {color_value} for click_tile")
        return False
    return True

# ---------------------------
# Solver logic functions
# Each takes a board (2D list) and returns a new board with moves applied.
# Board convention: -1 black, 1 white, 0 empty, None unknown/unreadable
# Convention is -1 black 1 white because we can take negatives to get opposite color easily.
# Additionally, taking sums is also easy to count
# ---------------------------

def copy_board(board):
    return [row[:] for row in board]

def apply_moves_and_clicks(moves, board):
    """
    moves: list of (r,c,value) to set
    Applies moves to the board (in memory) and physically clicks the screen.
    Returns modified board.
    """
    # reverse moves order
    #moves = list(reversed(moves))
    
    for r, c, v in moves:
        if board[r][c] in (-1, 1) and board[r][c] != v:
            # conflict: skip
            print(f"[MOVE] conflict at {(r,c)} skipping")
            continue
        click_tile(r, c, v)
        board[r][c] = v
        #print(f"[MOVE] placed {('black' if v==-1 else 'white')} at {(r,c)}")
    return board

def all_lines(n):
    """Return list of index lists for all rows and columns"""
    lines = []
    for r in range(n):
        lines.append([(r, c) for c in range(n)])   # row r
    for c in range(n):
        lines.append([(r, c) for r in range(n)])   # column c
    return lines

def line_values(board, indices):
    """Return list of values for a line given indices list [(r,c),...]"""
    return [board[r][c] for (r,c) in indices]

# 1) 2 in a row -> if there are two of the same color in line, the opposite color should be placed on both sides
def logic_two_in_a_row(board):
    n = len(board)
    moves = []
    # rows
    for r in range(n):
        for c in range(n - 2):
            left, mid, right = board[r][c], board[r][c+1], board[r][c+2]
            # pattern X X _
            if left == mid and left in (1, -1) and right == 0:
                moves.append((r, c+2, -left))
            # pattern _ X X
            if mid == right and mid in (1, -1) and left == 0:
                moves.append((r, c, -mid))
    # columns
    for c in range(n):
        for r in range(n - 2):
            top, mid, bot = board[r][c], board[r+1][c], board[r+2][c]
            if top == mid and top in (1, -1) and bot == 0:
                moves.append((r+2, c, -top))
            if mid == bot and mid in (1, -1) and top == 0:
                moves.append((r, c, -mid))
    # remove duplicates
    unique = list({(r,c,v) for (r,c,v) in moves})
    if overlay_visible and unique != []:
        print("[LOGIC] logic_two_in_a_row found moves:", unique)
    return unique

# 2) 1 gap -> two of same color on either side of empty tile -> fill with opposite
def logic_one_gap(board):
    n = len(board)
    moves = []
    # rows
    for r in range(n):
        for c in range(n - 2):
            left, mid, right = board[r][c], board[r][c+1], board[r][c+2]
            # pattern X _ X
            if left == right and left in (1, -1) and mid == 0:
                moves.append((r, c+1, -left))
    # columns
    for c in range(n):
        for r in range(n - 2):
            top, mid, bot = board[r][c], board[r+1][c], board[r+2][c]
            if top == bot and top in (1, -1) and mid == 0:
                moves.append((r+1, c, -top))
    unique = list({(r,c,v) for (r,c,v) in moves})
    if overlay_visible and unique != []:
        print("[LOGIC] logic_one_gap found moves:", unique)
    return unique

# 3) half satisfied line -> if a row/col of size n has n/2 of a color, all empty spaces fill with opposite color
def logic_half_satisfied(board):
    n = len(board)
    moves = []
    limit = n // 2
    lines = all_lines(n)
    for indices in lines:
        vals = line_values(board, indices)
        count_black = sum(1 for v in vals if v == -1)
        count_white = sum(1 for v in vals if v == 1)
        if count_black == limit:
            # place white (opposite) in all empties
            for (r,c), v in zip(indices, vals):
                if v == 0:
                    moves.append((r,c,1))
        if count_white == limit:
            # place black (opposite) in all empties
            for (r,c), v in zip(indices, vals):
                if v == 0:
                    moves.append((r,c,-1))
    unique = list({(r,c,v) for (r,c,v) in moves})
    if overlay_visible and unique != []:
        print("[LOGIC] logic_half_satisfied found moves:", unique)
    return unique

# 4) one-off combinational check (hardest)
def logic_one_off(board):
    """
    For any line (row or column) of length n:
      If there are (n/2)-1 of color A, and less than (n/2)-1 of color B,
      consider all placements of the *one additional* A into each empty (one at a time),
      then fill remaining empties in that line to reach counts that don't violate 3-in-row,
      collect all possible valid completions, then for positions that have the same color across
      all valid completions, set them.
    This is a conservative deduction algorithm.
    """
    n = len(board)
    moves = []
    limit = n // 2
    # for each line (row or column) count colors
    lines = all_lines(n)
    for indices in lines:
        vals = line_values(board, indices)
        count_black = sum(1 for v in vals if v == -1)
        count_white = sum(1 for v in vals if v == 1)
        count_empty = sum(1 for v in vals if v == 0)
        if count_black == limit - 1 and count_white < limit - 1 and count_empty > 0:
            target_color = -1  # black needs one more
        elif count_white == limit - 1 and count_black < limit - 1 and count_empty > 0:
            target_color = 1  # white needs one more
        else:
            continue
        # Try placing the target_color in each empty position one at a time, then filling empty spaces with the opposite color, and saving valid states
        empty_positions = [(r,c) for (r,c), v in zip(indices, vals) if v == 0]
        valid_boards = []
        for pos in empty_positions:
            # Create a new board state with the target_color placed
            new_board = deepcopy(board)
            new_board[pos[0]][pos[1]] = target_color
            # Fill remaining empty spaces with the opposite color
            for (r,c), v in zip(indices, line_values(new_board, indices)):
                if v == 0:
                    new_board[r][c] = -target_color
            # Check if the new board is valid
            if is_valid_board(new_board):
                valid_boards.append(new_board)
        if not valid_boards:
            continue
        # For each position in the line, if all valid boards have the same color, we can set it
        for i, (r,c) in enumerate(indices):
            colors_at_pos = {b[r][c] for b in valid_boards}
            if len(colors_at_pos) == 1:
                v = colors_at_pos.pop()
                if board[r][c] != v:
                    moves.append((r,c,v))
    unique = list({(r,c,v) for (r,c,v) in moves})
    if overlay_visible and unique != []:
        print("[LOGIC] logic_one_off found moves:", unique)
    return unique

def is_valid_board(board):
    """
    Check if the board has no immediate violations of the 3-in-a-row rule and color counts.
    """
    n = len(board)
    limit = n // 2
    lines = all_lines(n)
    for indices in lines:
        vals = line_values(board, indices)
        count_black = sum(1 for v in vals if v == -1)
        count_white = sum(1 for v in vals if v == 1)
        # Check counts
        if count_black > limit or count_white > limit:
            return False
        # Check for 3 in a row
        for i in range(n - 2):
            if vals[i] == vals[i+1] == vals[i+2] and vals[i] in (-1, 1):
                return False
    return True

# ---------------------------
# Solver loop
# ---------------------------

LOGIC_FUNCTIONS = [
    logic_two_in_a_row,
    logic_one_gap,
    logic_half_satisfied,
    logic_one_off
]

def run_solver_loop():
    """
    Main solver loop runs while state['running'] is True.
    It rescans board, repeatedly applies logic functions until no more moves,
    makes moves physically via clicks, then rescans until puzzle is complete or paused.
    """
    print("[RUN] Solver thread started.")

    while True:
        with lock:
            if state["killed"]:
                print("[RUN] killed flag set, exiting solver thread.")
                return
            running = state["running"]
        if not running:
            time.sleep(0.2)
            continue

        board = None
        if state["board"] is not None:
            board = deepcopy(state["board"])
            print("[RUN] Using existing board state in memory.")
        if board is None:
            board = scan_board()
            if board is None:
                print("[RUN] Cannot start solver: board not calibrated.")
                with lock:
                    state["running"] = False
                return

        n = len(board)
        progress_made = False
        # loop applying logic functions until no change
        while True:
            any_moves = []
            for func in LOGIC_FUNCTIONS:
                moves = func(board)
                if moves:
                    # filter out moves that conflict with known values or are duplicates
                    filtered = []
                    for r,c,v in moves:
                        if board[r][c] in (-1,1) and board[r][c] != v:
                            continue
                        if board[r][c] == v:
                            continue
                        filtered.append((r,c,v))
                    if filtered:
                        any_moves.extend(filtered)
            if not any_moves:
                break
            # apply unique set of moves
            unique_moves = list({(r,c,v) for (r,c,v) in any_moves})
            board = apply_moves_and_clicks(unique_moves, board)
            progress_made = True

        # check if board full or no progress
        empties = sum(1 for r in range(n) for c in range(n) if board[r][c] == 0)
        unknowns = sum(1 for r in range(n) for c in range(n) if board[r][c] is None)
        if overlay_visible:
            print(f"[RUN] Round complete: empties={empties}, unknowns={unknowns}")
            print_colored_board(board)
        state["board"] = deepcopy(board)
        if empties == 0 and unknowns == 0:
            # print("[RUN] Puzzle complete (no empties/unknowns). Stopping.")
            # with lock:
            #     state["running"] = False
            # reset all variables for next run
            print("[RUN] Press F8 to reuse the same settings")
            break
        # if we made no progress, pause and wait for user or next scan
        if not progress_made:
            print("[RUN] No deterministic progress possible currently. Pausing solver.")
            with lock:
                state["running"] = False
            break

# ---------------------------
# Overlay window using tkinter
# ---------------------------
# Best-effort transparent overlay. Behavior differs across OSes. This draws circles on tile centers and updates periodically.
try:
    import tkinter as tk
    from tkinter import Toplevel
    overlay_root = None
    overlay_canvas = None
    overlay_thread_obj = None
    overlay_stop_event = threading.Event()
except Exception as e:
    print("[OVERLAY] tkinter unavailable:", e)
    overlay_root = None

def start_overlay_loop():
    """
    Build and run a tkinter top-level window as overlay.
    This is a best-effort; transparency behavior differs by OS.
    """
    global overlay_root, overlay_canvas, overlay_thread_obj, overlay_stop_event
    if overlay_root is not None:
        return
    overlay_stop_event.clear()

    def overlay_thread():
        global overlay_root, overlay_canvas
        overlay_root = tk.Tk()
        overlay_root.title("Three-in-a-row Overlay")
        overlay_root.attributes("-topmost", True)
        # try to make window borderless and transparent - works best on Windows
        overlay_root.overrideredirect(True)
        # Make background color unique and set as transparent if supported
        transparent_color = "magenta"
        overlay_root.configure(bg=transparent_color)
        try:
            overlay_root.wm_attributes("-transparentcolor", transparent_color)
        except Exception:
            pass
        # full screen:
        w = overlay_root.winfo_screenwidth()
        h = overlay_root.winfo_screenheight()
        overlay_root.geometry(f"{w}x{h}+0+0")
        overlay_canvas = tk.Canvas(overlay_root, width=w, height=h, highlightthickness=0, bg=transparent_color)
        overlay_canvas.pack()
        # draw loop
        def redraw():
            overlay_canvas.delete("all")
            with lock:
                centers = state.get("board_centers")
                board = state.get("last_scan")
            if centers and board:
                n = len(board)
                for r in range(n):
                    for c in range(n):
                        cx, cy = centers[r][c]
                        # quarter-size circle
                        if state["tile_dx"] is not None:
                            radius = max(6, int(abs(state["tile_dx"]) * 0.25))
                        else:
                            radius = 10
                        left = cx - radius
                        top = cy - radius
                        right = cx + radius
                        bottom = cy + radius
                        val = board[r][c]
                        if val == 0:
                            color = "red"   # empty
                        elif val == 1:
                            color = "green" # white
                        elif val == -1:
                            color = "blue"  # black
                        else:
                            color = "yellow"
                        overlay_canvas.create_oval(left, top, right, bottom, fill=color, outline="")
            if not overlay_stop_event.is_set():
                overlay_root.after(200, redraw)
            else:
                try:
                    overlay_root.destroy()
                except:
                    pass
        overlay_root.after(100, redraw)
        overlay_root.mainloop()

    overlay_thread_obj = threading.Thread(target=overlay_thread, daemon=True)
    overlay_thread_obj.start()
    print("[OVERLAY] started")

def stop_overlay():
    global overlay_stop_event
    overlay_stop_event.set()
    print("[OVERLAY] stopping")

def toggle_overlay():
    global overlay_root, overlay_canvas, overlay_visible
    if overlay_visible:
        if overlay_root:
            overlay_root.destroy()
        overlay_root = None
        overlay_canvas = None
        overlay_visible = False
    else:
        import tkinter as tk
        overlay_root = tk.Tk()
        overlay_root.attributes("-topmost", True)
        overlay_root.attributes("-transparentcolor", "white")
        overlay_root.overrideredirect(True)
        overlay_canvas = tk.Canvas(overlay_root, bg="white", highlightthickness=0)
        overlay_canvas.pack(fill="both", expand=True)
        overlay_visible = True

# ---------------------------
# Hotkey callbacks and listeners
# ---------------------------
# We'll use pynput.keyboard.GlobalHotKeys to map function keys to callbacks.
# Because we need the mouse position at the time of the F-key press, we'll record current mouse position inside each callback.

def on_press_f2():
    print("[HOTKEY] F2 pressed (start diagonal)")
    pos = get_mouse_position()
    #clear all existing calibration data
    state["diag_points"] = []
    state["top_left"] = None
    state["bottom_right"] = None
    state["n"] = None
    state["tile_dx"] = None
    state["tile_dy"] = None
    state["board_centers"] = []
    state["last_scan"] = None
    state["board"] = None
    # record as starting diagonal point
    record_diagonal_point(pos)

def on_press_f3():
    print("[HOTKEY] F3 pressed (diagonal intermediate)")
    pos = get_mouse_position()
    record_diagonal_point(pos)

def on_press_f4():
    print("[HOTKEY] F4 pressed (final diagonal)")
    pos = get_mouse_position()
    record_diagonal_point(pos)  # include final point too
    finalize_diagonal()
    scan_board()  # initial scan after calibration
    state["running"] = True  # start running after calibration

def on_press_f1():
    print("[HOTKEY] F1 pressed (color sample)")
    pos = get_mouse_position()
    sample_color_sequence(pos)
    print(pos)

def on_press_f5():
    print("[HOTKEY] F5 pressed (overlay toggle)")
    toggle_overlay()

TEMP_INTERP = INTERP_STEP
TEMP_CLICK_DELAY = CLICK_DELAY
TEMP_INTERP_VELOCITY = INTERP_VELOCITY
def on_press_f8():
    state["autoplay"] = not state["autoplay"]
    print(f"[HOTKEY] F8 pressed (autoplay toggled to {state['autoplay']})")
    if state["autoplay"]:
        global TEMP_INTERP, TEMP_CLICK_DELAY, INTERP_STEP, CLICK_DELAY, TEMP_INTERP_VELOCITY, INTERP_VELOCITY
        INTERP_STEP = TEMP_INTERP
        CLICK_DELAY = TEMP_CLICK_DELAY
        INTERP_VELOCITY = TEMP_INTERP_VELOCITY
        threading.Thread(target=autoplay, daemon=True).start()
    else: 
        # auto kill autoplayer
        state["running"] = False
        INTERP_STEP = 0
        CLICK_DELAY = 0.00001
        INTERP_VELOCITY = 0
        
def autoplay():
    while state["autoplay"]:
        print("[HOTKEY] F8 pressed (autoplay on)")
        # Make keyboard do Ctrl R
        state["board"] = None
        scan_board()
        state["running"] = True
        run_solver_loop()
        print("[HOTKEY] F8 done")
        keyboard_controller.press(keyboard.Key.ctrl)
        keyboard_controller.press('r')
        keyboard_controller.release('r')
        keyboard_controller.release(keyboard.Key.ctrl)
        time.sleep(REFRESH_DELAY)

# def on_press_f8():
#     with lock:
#         state["running"] = not state["running"]
#     print(f"[HOTKEY] F8 pressed (running toggled to {state['running']})")

def on_press_f9():
    with lock:
        state["killed"] = True
        state["running"] = False
    print("[HOTKEY] F9 pressed (kill). Exiting.")
    # stop overlay if running
    stop_overlay()
    # stop listeners
    for l in listener_threads:
        try:
            l.stop()
        except Exception:
            pass
        


# Map keys to functions
HOTKEY_MAP = {
    "<f1>": on_press_f1,
    "<f2>": on_press_f2,
    "<f3>": on_press_f3,
    "<f4>": on_press_f4,
    "<f5>": on_press_f5,
    #"<f6>": on_press_f6,
    "<f8>": on_press_f8,
    "<f9>": on_press_f9,
    # "<1>": on_press_f1,  # for testing with number keys
    # "<2>": on_press_f2,
    # "<3>": on_press_f3,
    # "<4>": on_press_f4,
    # "<5>": on_press_f5, 
    #"<8>": on_press_f8, # is actually backspace
    #"<9>": on_press_f9, # is actually tab
    # "<31>": on_press_f1,  # '1' key
    # "<32>": on_press_f2,  # '2' key actually space??
    # "<33>": on_press_f3,  # '3' key
    # "<34>": on_press_f4,  # '4' key
    # "<35>": on_press_f5,  # '5' key
    # "<38>": on_press_f8,  # '8' key
    # "<39>": on_press_f9,  # '9' key
}

def start_hotkey_listener():
    # pynput's GlobalHotKeys uses string names like '<f1>' etc.
    from pynput.keyboard import GlobalHotKeys
    hotkey_map = {k: (lambda f=func: f()) for k, func in HOTKEY_MAP.items()}
    gh = GlobalHotKeys(hotkey_map)
    listener_threads.append(gh)
    gh.start()
    print("[HOTKEY] Global hotkeys started.")
    
def get_mouse_position():
    pos = mouse_controller.position
    override = list(pos)
    override[0] = 2 * override[0]
    override[1] = 2 * override[1]
    return tuple(override)

def print_colored_board(board):
    if board is None:
        print("No board")
        return
    n = len(board)
    for r in range(n):
        for c in range(n):
            v = board[r][c]
            if v == -1:
                print("\033[30m■\033[0m", end=" ")  # black square
            elif v == 1:
                print("\033[37m■\033[0m", end=" ")  # white square
            elif v == 0:
                print("□", end=" ")  # uncolored square for empty
            else:
                print("\033[31m■\033[0m", end=" ")  # unknown/error is red
        print()
    print()

# ---------------------------
# Main
# ---------------------------
def main():
    print("Three-in-a-row solver starting.")
    print("Calibration flow:")
    print("  1) Hover over a black tile, press F1; then over a white tile, press F1; then over an empty tile, press F1 (three F1 presses total).")
    print("  2) Hover over top-left tile center, press F2")
    print("  3) For each tile along the negative diagonal (top-left to bottom-right), excluding first and last corners, hover over its center and press F3")
    print("  3.5) Technically, you don't have to hover over the diagonals. It just counts the amount of intermediate tiles. You can press F3 multiple times anywhere on the screen to count the diagonal tiles that aren't the start and end corners to speed up the process a bit.")
    print("  4) Hover over final bottom-right diagonal tile and press F4")
    print("Overlay toggle: F5. Rerun solver: F8. Kill program: F9")

    start_hotkey_listener()

    solver_thread = threading.Thread(target=run_solver_loop, daemon=True)
    solver_thread.start()

    # Keep main thread alive until killed
    try:
        while True:
            with lock:
                if state["killed"]:
                    break
            time.sleep(0.2)
    except KeyboardInterrupt:
        print("KeyboardInterrupt: exiting.")
    finally:
        stop_overlay()
        print("Exiting program.")

if __name__ == "__main__":
    main()
