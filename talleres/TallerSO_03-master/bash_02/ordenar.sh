#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Uso: $0 <nombre_fichero> <columna>"
  exit 1
fi

archivo=$1
columna=$2

if [ ! -f "$archivo" ]; then
  echo "Error: el archivo no existe"
  exit 1
fi

cut -d',' -f"$columna" "$archivo" | grep -v '^$' | sort -r
