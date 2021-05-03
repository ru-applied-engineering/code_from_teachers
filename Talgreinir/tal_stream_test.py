#!/usr/bin/env python3
# MIT License
#
# Copyright (c) 2018 Róbert Kjaran <robert@kjaran.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Þessi kóði er í grunninn fenginn af https://github.com/cadia-lvl/tal.ru.is/tree/master/egs/py_apikey
# Breytingar gerðar af Davíð Frey Jónssyni. Bætti við straumi frá hljóðnema sem sendur er á tal.ru.is

from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
import base64
import os
import json
from time import sleep
from threading import Thread
import wave
from collections import OrderedDict
from ws4py.client.threadedclient import WebSocketClient
import pyaudio
import time

WIDTH = 2
CHANNELS = 1
RATE = 44100
WS_G = None
CHUNK=4096

TRANSCRIPT_F_PATH= "/home/pi/tal_transcipt" #"/dev/shm/tal_transcipt"



def callback(in_data, frame_count, time_info, status):
    global WS_G
    b64chunk = base64.b64encode(in_data)
    #print(b64chunk)
    
    WS_G.send(json.dumps(
        {'audioContent': b64chunk.decode('utf-8')}))
    #f = open("/dev/shm/demofile2.txt", "a")
    #f.write(b64chunk.decode('utf-8'))
    #f.close()
    return (in_data, pyaudio.paContinue)

class StreamingRecognizeClient(WebSocketClient):
    def __init__(self, wav_path, *args, **kwargs):
        self.wav_path = wav_path
        self.transcripts = OrderedDict()
        self.p = pyaudio.PyAudio()
        super(StreamingRecognizeClient, self).__init__(*args, **kwargs)

    

    def opened(self):
        def run():
            #with wave.open(self.wav_path, 'r') as wav:
               # sample_rate = wav.getframerate()
               # nframes = wav.getnframes()
               # chunk_width = 800
                stream = self.p.open(format=pyaudio.paInt16,
                    channels=CHANNELS,
                    rate=RATE,
                    #input_device_index = 0,
                    input=True,
                    stream_callback=callback,
                    frames_per_buffer=CHUNK)
                self.send(json.dumps(
                    {'streamingConfig': {'config': {'sampleRate': RATE,
                                                    'wordAlignment': True},
                                                    
                                         'interimResults': True}}))


                while True:

                    time.sleep(5)

                self.send(json.dumps({'audioContent': ''}))
        Thread(target=run).start()

    def closed(self, code, reason=None):
        print('')

    def received_message(self, m):
        try:
            #print("Reveived")
            response = json.loads(m.data.decode('utf-8'))
            
            transcript = response['result']['results'][0]['alternatives'][0]['transcript']
            #print(transcript)
            transcript_file = open(TRANSCRIPT_F_PATH, 'w')
            transcript_file.write(transcript)
            transcript_file.close()

            word=response['result']['results'][0]['alternatives'][0]['wordAlignment'][0]['word']
            result_index = response['result'].get('resultIndex', 0)
            is_final = response['result']['results'][0].get('isFinal', False)
            #print("\n----------RESPONSE----------")
            #print(response)
            #transcript_file = open(TRANSCRIPT_F_PATH, 'r')
            
            print("\n----------TRANSCRIPT----------")
            print(transcript)
            
            print("\n----------WORD----------")
            print(word)
            
            #print(transcript_file.read())
            print("\n")
            transcript_file.close()
            self.transcripts[result_index] = transcript
            
        except KeyError as e:
            pass

        #print('\r{}'.format(''.join(val for key, val in self.transcripts.items())),
              #end='', flush=True)


def parse_args():
    po = ArgumentParser(
        description="""Example usage of the Speech::StreamingRecognize API.  Set
        the environment variable TALA_API_KEY to the API key you generated at
        https://tal.ru.is/stjori
        """,
        formatter_class=ArgumentDefaultsHelpFormatter)
    po.add_argument('--uri',
                    default='wss://tal.ru.is/v1/speech:streamingrecognize',
                    help='URI of Speech::StreamingRecognize API endpoint.')
    po.add_argument('wav_path', type=str, help='WAV file to recognize.')
    args = po.parse_args()
    args.api_key = os.environ['TALA_API_KEY']

    return args


def main(args):
    global WS_G
    try:
        WS_G = StreamingRecognizeClient(
            args.wav_path, '{}?token={}'.format(args.uri, args.api_key))
        
        WS_G.connect()
        WS_G.run_forever()
    except KeyboardInterrupt:
        WS_G.close()


if __name__ == '__main__':
    args = parse_args()
    main(args)