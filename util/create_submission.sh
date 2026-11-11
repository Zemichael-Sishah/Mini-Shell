#!/bin/bash

# in case any errors
set -euo pipefail

# the files we want to tar
FILES=("msh_execute.c" "mshparse/msh_parse.c")

# iterate through the files and make sure they exist
for f in "${FILES[@]}"; do
    if [[ ! -f "$f" ]]; then
        echo "Error: $f not found." >&2
        exit 1
    fi
done

# tar them with the COPY_DISABLE=1 argument to exclude garbage metadata
# COPY_DISABLE=1 tar -cvf handin.tgz "${FILES[@]}"