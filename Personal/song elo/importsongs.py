"""DOCSTRING???"""

import csv
# Admin cmd: pip install ytmusicapi
# Admin cmd: ytmusicapi oauth, follow instructions
from ytmusicapi import YTMusic

# ask user for oauth.json or browser.json path, as well as the songs.csv path
auth_path = input("Enter the path to your oauth.json or browser.json file: ")
csv_path = input("Enter the path to your songs.csv file: ")

music = YTMusic(auth_path)

# Get liked songs
liked_songs = music.get_playlist("LM", 4000)

# store liked songs in songs.csv
with open(csv_path, "w", newline="", encoding='utf-8') as f:
    writer = csv.writer(f)
    for song in liked_songs['tracks']:
        title = song['title']
        # filter out commas in song titles
        title = title.replace(',', ' ')
        artist = song['artists'][0]['name']
        # filter out commas in artist names
        artist = artist.replace(',', ' ')
        link = song['videoId']
        writer.writerow([title, artist, 100, 0, link])
