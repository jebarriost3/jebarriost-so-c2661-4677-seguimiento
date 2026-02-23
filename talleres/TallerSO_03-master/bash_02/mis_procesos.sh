#!/bin/bash
usuario="${USERNAME:-$USER}"
ps -u "$usuario" -f
