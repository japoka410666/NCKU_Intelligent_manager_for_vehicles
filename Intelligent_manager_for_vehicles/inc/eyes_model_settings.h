/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_HANDWRITING_MODEL_SETTINGS_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_HANDWRITING_MODEL_SETTINGS_H_

constexpr int kNumCols_2 = 64;
constexpr int kNumRows_2 = 64;
constexpr int kNumChannels_2 = 1;

constexpr int kMaxImageSize_2 = kNumCols_2 * kNumRows_2 * kNumChannels_2;

constexpr int kCategoryCount_2 = 2;

/////////////////////
constexpr int kopeneyeIndex =1;
constexpr int kcloseeyeIndex = 0;

//////////////////////

extern const char* kCategoryLabels_2[kCategoryCount_2];

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_HANDWRITING_MODEL_SETTINGS_H_
