"""DOCSTRING???"""

import csv
# Admin cmd: pip install ytmusicapi
# Admin cmd: ytmusicapi oauth, follow instructions
from ytmusicapi import YTMusic

music = YTMusic("C:/Users/User/Documents/GitHub/CPSC-coursework/Personal/song elo/browser.json")

# Get liked songs
liked_songs = music.get_playlist("LM", 4000)

# store liked songs in songs.csv
with open("C:/Users/User/Documents/GitHub/CPSC-coursework/Personal/song elo/songs.csv", "w", newline="", encoding='utf-8') as f:
    writer = csv.writer(f)
    for song in liked_songs['tracks']:
        title = song['title']
        artist = song['artists'][0]['name']
        link = song['videoId']
        writer.writerow([title, artist, 100, 0, link])
