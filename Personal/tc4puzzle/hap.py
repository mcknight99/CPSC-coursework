"""..."""
import os
import pygame
import pygame_gui
from pygame_gui.elements import UIButton, UILabel, UIPanel


# Initialize pygame and pygame_gui
pygame.init()
print(pygame)
print(dir(pygame))

# Constants
SCREEN_WIDTH, SCREEN_HEIGHT = 800, 600
HEX_SIZE = 40  # Adjust hex size as needed
BG_COLOR = (30, 30, 30)
FONT = pygame.font.Font(None, 36)

# Load aspect relations from file
def load_aspect_relations(file_path):
    aspects = {}
    try:
        with open(file_path, 'r') as file:
            lines = file.read().splitlines()
            for line in lines:
                if '+' in line and '=' in line:
                    parts = line.split('=')
                    result = parts[1].strip()
                    components = tuple(parts[0].split('+'))
                    aspects[result] = components
                else:
                    aspects[line.strip()] = ()
    except FileNotFoundError:
        print("Aspect file not found!")
    return aspects

ASPECTS = load_aspect_relations("aspects.txt")

# Load tile images
def load_tile_image(aspect_name):
    image_path = os.path.join("images", f"{aspect_name}.png")
    if os.path.exists(image_path):
        return pygame.image.load(image_path)
    return None

# Hex Grid Class
# switch to https://github.com/fananek/hex-grid
class HexGrid:
    def __init__(self, rows, cols):
        self.rows = rows
        self.cols = cols
        self.grid = [[None for _ in range(cols)] for _ in range(rows)]
    
    def draw(self, screen):
        for row in range(self.rows):
            for col in range(self.cols):
                x, y = self.get_hex_position(row, col)
                pygame.draw.polygon(screen, (100, 100, 100), self.get_hex_points(x, y), 1)
    
    def get_hex_position(self, row, col):
        x = col * HEX_SIZE * 1.5
        y = row * HEX_SIZE * (3 ** 0.5)
        if col % 2 == 1:
            y += HEX_SIZE * (3 ** 0.5) / 2
        return x + 100, y + 100  # Offset for visibility
    
    def get_hex_points(self, x, y):
        return [(x + HEX_SIZE * 0.5 * dx, y + HEX_SIZE * 0.5 * dy) for dx, dy in 
                [(2, 0), (1, 1.732), (-1, 1.732), (-2, 0), (-1, -1.732), (1, -1.732)]]

# Game Setup
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
manager = pygame_gui.UIManager((SCREEN_WIDTH, SCREEN_HEIGHT))
running = True
clock = pygame.time.Clock()
hex_grid = HexGrid(5, 5)

# Game Loop
while running:
    time_delta = clock.tick(60) / 1000.0
    screen.fill(BG_COLOR)
    hex_grid.draw(screen)
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        manager.process_events(event)
    
    manager.update(time_delta)
    manager.draw_ui(screen)
    pygame.display.flip()

pygame.quit()
