#!/usr/bin/env bash

die () {
    echo >&2 "$@"
    exit 22
}

[ "$#" -gt 0 ] || die "$0:ERROR:commit message required:$# arguments provided"

make
git add .
git commit -m "${*}"
git pull --ff-only || echo Pull failed with $?
git push
