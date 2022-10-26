#!/bin/sh

base64 /dev/urandom | head -c $1 | egrep -ao "\w" | tr -d '\n' > $2
