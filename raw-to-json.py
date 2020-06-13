#!/usr/bin/env python3
"""
This script turns the raw output created by `dumpmsg` into JSON.

We assume that message bodies are valid UTF-8.

Example usage:
./treomsg/dumpmsg -r *.pdr > out.txt
./raw-to-json.py out.txt some-json-file.json
"""
import sys
import json
import base64
from collections import namedtuple

def empty_message():
    return {
        'filename': '', # string
        'incoming': False, # boolean
        'phone': None, # ?string
        'phone_name': None, # ?string
        # These are unix timestamps
        'dates': [], # number[]
        'body': '', # string
    }

def extract_messages(filename):
    """
    Given a filename, open it and attempt to interpret its contents as a
    sequence of `dumpmsg -r` message outputs and return a list of messages.
    """

    messages = []

    message = None
    with open(filename, "rb") as f:
        line = ''
        while True:
            byte = f.read(1)

            if byte == b"":
                break

            # Build up the current line we're on
            if byte != b'\n':
                try:
                    line += byte.decode('latin1')
                except UnicodeDecodeError:
                    pass
                continue

            # We've reached a newline. Let's see what we have here.

            # We probably don't care about this. Might just be some junk that
            # got in somehow.
            if not ': ' in line and not message or not line:
                line = ''
                continue

            if line.startswith('block'):
                try:
                    num_bytes = int(line.split(' ')[-2])
                    block = f.read(num_bytes)
                    message['body'] = message['body'] + block.decode('latin1')
                except UnicodeDecodeError as e:
                    print(block)
                    if message:
                        print('failed to decode %s' % (message['filename']))
                finally:
                    line = ''
                continue

            field, rest = line.split(': ')

            if field == 'InputFileName':
                if message:
                    messages.append(message)
                message = empty_message()
                message['filename'] = rest
            elif field == 'InOutFlag':
                # This happens for unreadable data blocks
                if rest == '0':
                    message = None
                else:
                    message['incoming'] = 'incoming' in rest
            elif field == 'PhoneNumber':
                message['phone'] = rest
            elif field == 'PhoneName':
                message['phone_name'] = rest
            elif field.startswith('Date'):
                message['dates'].append(int(rest))

            line = ''

    return messages


if __name__ == "__main__":
    args = sys.argv[1:]

    if len(args) != 2:
        print('You must provide an input file and an output file.')
        exit(1)

    infile, outfile = args
    messages = extract_messages(infile)
    jsonified = json.dumps(messages, indent=4, sort_keys=True)

    with open(outfile, 'w') as out:
        out.write(jsonified)
