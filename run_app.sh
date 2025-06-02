#!/bin/bash
cd "$(dirname "$0")"
if [[ -f "build/ImageSequenceConverter.app/Contents/MacOS/ImageSequenceConverter" ]]; then
    open build/ImageSequenceConverter.app
else
    echo "Application not built. Please run build_and_run.sh first."
    exit 1
fi
