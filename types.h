#pragma once
#include <Arduino.h>

#include <stddef.h>

typedef unsigned char byte;
typedef void (*MidiCallback)(byte channel, byte note, byte velocity);

typedef void (*ValueMutator)(byte);
