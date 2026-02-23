#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Uso: $0 <directorio_trabajo>"
  exit 1
fi

directorio=$1

if [ ! -d "$directorio" ]; then
  echo "Error: el directorio no existe"
  exit 1
fi

find "$directorio" -name ".keep" -print0 | xargs -0 git rm -f
