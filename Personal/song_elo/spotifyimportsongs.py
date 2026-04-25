#spotipy 
# do this same thing but for spotify
# """DOCSTRING???"""

# import csv
# # Admin cmd: pip install ytmusicapi
# # Admin cmd: ytmusicapi oauth, follow instructions
# from ytmusicapi import YTMusic

# # ask user for oauth.json or browser.json path, as well as the songs.csv path
# auth_path = input("Enter the path to your oauth.json or browser.json file: ")
# csv_path = input("Enter the path to your songs.csv file: ")

# music = YTMusic(auth_path)

# # Get liked songs
# liked_songs = music.get_playlist("LM", 4000)

# # store liked songs in songs.csv
# with open(csv_path, "w", newline="", encoding='utf-8') as f:
#     writer = csv.writer(f)
#     for song in liked_songs['tracks']:
#         title = song['title']
#         # filter out commas in song titles
#         title = title.replace(',', ' ')
#         artist = song['artists'][0]['name']
#         # filter out commas in artist names
#         artist = artist.replace(',', ' ')
#         link = song['videoId']
#         writer.writerow([title, artist, 100, 0, link])


"""Imports songs from a public Spotify playlist and saves them to a CSV file."""
import csv
import spotipy
from spotipy.oauth2 import SpotifyClientCredentials
# Ask user for Spotify playlist URL and CSV file path
playlist_url = input("Enter the Spotify playlist URL: ")
csv_path = input("Enter the path to your songs.csv file: ")
# Set up Spotify client credentials (replace with your own client ID and secret)
client_id = input("Enter your Spotify Client ID: ")
client_secret = input("Enter your Spotify Client Secret: ")
sp = spotipy.Spotify(auth_manager=SpotifyClientCredentials(client_id=client_id, client_secret=client_secret))
# Extract playlist ID from URL
playlist_id = playlist_url.split("/")[-1].split("?")[0]
# Get playlist tracks
results = sp.playlist_items(playlist_id)
# Store songs in songs.csv
with open(csv_path, "w", newline="", encoding='utf-8') as f:
    writer = csv.writer(f)
    for item in results['items']:
        track = item['track']
        title = track['name'].replace(',', ' ')  # Filter out commas in song titles
        artist = track['artists'][0]['name'].replace(',', ' ')  # Filter out commas in artist names
        link = track['id']
        writer.writerow([title, artist, 100, 0, link])
        