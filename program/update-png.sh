#!/usr/bin/env bash
app="okteta"
for i in 16 22 32 48 64 128; do
    ksvgtopng5 $i $i $app.svg $i-apps-$app.png
done
