import speech_recognition as sr
from textblob import TextBlob
import RPi.GPIO as GPIO
import time

# GPIO setup
red_led_pin = 17
yellow_led_pin = 27
green_led_pin = 22

GPIO.setmode(GPIO.BCM)
GPIO.setup(red_led_pin, GPIO.OUT)
GPIO.setup(yellow_led_pin, GPIO.OUT)
GPIO.setup(green_led_pin, GPIO.OUT)

def analyze_sentiment(text):
    """
    Analyze the sentiment of the given text using TextBlob.
    Returns 'green' for positive sentiment, 'yellow' for neutral, and 'red' for negative sentiment.
    """
    blob = TextBlob(text)
    polarity = blob.sentiment.polarity
    if polarity > 0:
        return 'green'
    elif polarity == 0:
        return 'yellow'
    else:
        return 'red'

def control_leds(color):
    """
    Control the traffic light LEDs based on the sentiment analysis result.
    Turns on one LED corresponding to the sentiment color and turns off the others.
    """
    GPIO.output(red_led_pin, GPIO.LOW)
    GPIO.output(yellow_led_pin, GPIO.LOW)
    GPIO.output(green_led_pin, GPIO.LOW)
    
    if color == 'red':
        GPIO.output(red_led_pin, GPIO.HIGH)
    elif color == 'yellow':
        GPIO.output(yellow_led_pin, GPIO.HIGH)
    elif color == 'green':
        GPIO.output(green_led_pin, GPIO.HIGH)

def listen_and_respond():
    """
    Listen for a voice input using the microphone, analyze the sentiment of the recognized text,
    and control the traffic light LEDs based on the sentiment.
    """
    recognizer = sr.Recognizer()
    mic = sr.Microphone()

    with mic as source:
        print("Listening...")
        audio = recognizer.listen(source)
    
    try:
        # Recognize speech using Google Web Speech API
        text = recognizer.recognize_google(audio)
        print(f"You said: {text}")

        # Analyze sentiment of the recognized text
        sentiment = analyze_sentiment(text)
        print(f"Sentiment: {sentiment}")

        # Control LEDs based on sentiment
        control_leds(sentiment)

    except sr.UnknownValueError:
        print("Sorry, I could not understand the audio.")
    except sr.RequestError as e:
        print(f"Could not request results; {e}")

if __name__ == "__main__":
    try:
        while True:
            listen_and_respond()
            time.sleep(1)  # Add a short delay to avoid spamming the recognizer
    except KeyboardInterrupt:
        print("Exiting program...")
    finally:
        GPIO.cleanup()  # Clean up GPIO pins on exit
