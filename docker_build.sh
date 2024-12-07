#!/bin/bash

docker run --rm -it -v $(pwd):/src -w /src ev3cc make all