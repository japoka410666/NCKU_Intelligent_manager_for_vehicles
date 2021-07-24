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

#include "main_functions.h"
#include "image_provider.h"
#include "eyes_model_data.h"
#include "eyes_model_settings.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"


namespace {
tflite::ErrorReporter* error_reporter = nullptr;

// Declaration for eyes model
const tflite::Model* eyes_model = nullptr;
tflite::MicroInterpreter* interpreter2 = nullptr;
TfLiteTensor* eyes_input = nullptr;
TfLiteTensor* eyes_output = nullptr;

// An area of memory to use for input, output, and intermediate arrays.
constexpr int kTensorArenaSize = 160 * 1024;
#if (defined(__GNUC__) || defined(__GNUG__)) && !defined (__CCAC__)
static uint8_t tensor_arena[kTensorArenaSize] __attribute__((section(".tensor_arena")));
#else
#pragma Bss(".tensor_arena")
static uint8_t tensor_arena[kTensorArenaSize];
#pragma Bss()
#endif // if defined (_GNUC_) && !defined (_CCAC_)
}  // namespace

void setup() {
  
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  /************* EYES MODEL SETTING*************/
  eyes_model = tflite::GetModel(eyes_model_int8_tflite);
  if (eyes_model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         eyes_model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::MicroMutableOpResolver<6> micro_op_resolver2;
  micro_op_resolver2.AddConv2D();
  micro_op_resolver2.AddRelu();
  micro_op_resolver2.AddMaxPool2D();
  micro_op_resolver2.AddFullyConnected();
  micro_op_resolver2.AddReshape();
  micro_op_resolver2.AddSoftmax();
 
  static tflite::MicroInterpreter static_interpreter2(
      eyes_model, micro_op_resolver2, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter2 = &static_interpreter2;

  TfLiteStatus allocate_status2 = interpreter2->AllocateTensors();
  if (allocate_status2 != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors2() failed");
    return;
  }

  eyes_input = interpreter2->input(0);
  eyes_output = interpreter2->output(0);
  /*************END EYES MODEL SETTING*************/
}

int eyes_loop() {
  float output_scale = eyes_output->params.scale;
  int32_t output_zero_point = eyes_output->params.zero_point;

  // Get image from provider, which is camera.
  if (kTfLiteOk != GetImage(error_reporter, kNumCols_2, kNumRows_2, kNumChannels_2,
                            eyes_input->data.int8)) {
    TF_LITE_REPORT_ERROR(error_reporter, "Image capture failed.");
  }

  if (kTfLiteOk != interpreter2->Invoke()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
  }

  int8_t* results_ptr = eyes_output->data.int8;
  int result = std::distance(results_ptr,std::max_element(results_ptr,results_ptr+3));
  float confidence = ( (results_ptr[result] - output_zero_point ) * output_scale+ 1 ) / 2;

  TF_LITE_REPORT_ERROR(error_reporter,"Predicted:");

    //confidence too low
  if(confidence * 100 < 60){
    TF_LITE_REPORT_ERROR(error_reporter,"Not Sure");
    return 2;
  }

  else{
    if(result == 0){
      TF_LITE_REPORT_ERROR(error_reporter,"Closed (%d%%)", (int)(confidence * 100));
    }
    else if(result == 1){
      TF_LITE_REPORT_ERROR(error_reporter,"Open (%d%%)", (int)(confidence * 100));
    }
    else if(result == 2){
      TF_LITE_REPORT_ERROR(error_reporter,"Stranger (%d%%)", (int)(confidence * 100));
    }
    //Predict Closed Eyes
    if (result == 0){
      return 0;
    }
    //Predict Opened Eyes
    else if (result == 1) {
      return 1;
    }
    //Predict Stranger
    else if(result == 2){
      return 3;
    }
  }
}