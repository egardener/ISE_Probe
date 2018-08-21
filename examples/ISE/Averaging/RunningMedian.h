// Copyright (c) 2018 Justin Decker

//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// This is a heavily modified version of Rob Tillaart's Running Median class
// FILE: RunningMedian.h
// AUTHOR: Rob dot Tillaart at gmail dot com
// PURPOSE: RunningMedian library for Arduino
// VERSION: 0.1.14
//     URL: http://arduino.cc/playground/Main/RunningMedian
//
// Released to the public domain

#ifndef RunningMedian_h
#define RunningMedian_h

#include "Arduino.h"
#include <inttypes.h>

#define MEDIAN_MIN_SIZE     1
#define MEDIAN_MAX_SIZE     19

class RunningMedian {
public:

  RunningMedian(const uint8_t size)
  {
    _size = constrain(size, MEDIAN_MIN_SIZE, MEDIAN_MAX_SIZE);
    clear();
  }

  RunningMedian()
  {
    _size = 19;
    clear();
  }

  void clear()
  {
    _cnt    = 0;
    _idx    = 0;
    _sorted = false;

    for (uint8_t i = 0; i < _size; i++) _p[i] = i;
  }

  void add(float value)
  {
    _ar[_idx++] = value;
    if (_idx >= _size) _idx = 0;  // wrap around
    if (_cnt < _size) _cnt++;
    _sorted = false;
  }

  float getAverage() {
    return getAverage(9);
  }

  float getAverage(uint8_t nMedians)
  {
    if ((_cnt == 0) || (nMedians == 0)) return NAN;

    if (_cnt < nMedians) nMedians = _cnt;  // when filling the array for first time
    uint8_t start = ((_cnt - nMedians) / 2);
    uint8_t stop  = start + nMedians;

    if (_sorted == false) sort();

    float sum = 0;

    for (uint8_t i = start; i < stop; i++) sum += _ar[_p[i]];
    return sum / nMedians;
  }

  float round0(float i) {
    return floor(i * 10.0 + 0.5) / 10.0;
  }

  float round00(float i) {
    return floor(i * 100.0 + 0.5) / 100.0;
  }

  float round000(float i) {
    return floor(i * 1000.0 + 0.5) / 1000.0;
  }

protected:

  boolean _sorted;
  uint8_t _size;
  uint8_t _cnt;
  uint8_t _idx;
  float _ar[MEDIAN_MAX_SIZE];
  uint8_t _p[MEDIAN_MAX_SIZE];
  void sort()
  {
    for (uint8_t i = 0; i < _cnt - 1; i++)
    {
      bool flag = true;

      for (uint8_t j = 1; j < _cnt - i; j++)
      {
        if (_ar[_p[j - 1]] > _ar[_p[j]])
        {
          uint8_t t = _p[j - 1];
          _p[j - 1] = _p[j];
          _p[j]     = t;
          flag      = false;
        }
      }
      if (flag) break;
    }
    _sorted = true;
  }
};

#endif // ifndef RunningMedian_h
