#!/bin/bash

# Get the location of the current script
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )";

# Delete the previous export statement from .zshrc
sed -i '' '/AZTEKAS_PATH/d' $HOME/.zshrc

# Write the new export path
EXPORT_STATEMENT="export AZTEKAS_PATH=$SCRIPT_DIR"
echo $EXPORT_STATEMENT >> $HOME/.zshrc

# Source .zshrc again
source $HOME/.zshrc
