/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
//  @author sgazeos@gmail.com
//

#include <op_boilerplate.h>
#if NOT_EXCLUDED(OP_bincount)

//#include <ops/declarable/headers/parity_ops.h>
#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(resize_linear, 1, 1, false, 0, -2) {

            NDArray<T>* values = INPUT_VARIABLE(0);
            
            int width;
            int height;
            if (block.width() > 1) {
                auto newImageSize = INPUT_VARIABLE(1);
                REQUIRE_TRUE(newImageSize->lengthOf() == 2, 0, "resize_linear: Resize params is a pair of values, not %i.", newImageSize->lengthOf());
                REQUIRE_TRUE(block.numI() == 0, 0, "resize_linear: Resize params already given by the second param. Int params are expensive.");
                width = int(newImageSize->getScalar(0));
                height = int(newImageSize->getScalar(1));
            }
            else {
                REQUIRE_TRUE(block.numI() == 2, 0, "resize_linear: Neither resize width nor height are provided.");
                width = INT_ARG(0);
                height = INT_ARG(1);
            }

            return ND4J_STATUS_OK;
        }

        DECLARE_SHAPE_FN(resize_linear) {
            auto shapeList = SHAPELIST(); 
            auto in = inputShape->at(0);

            Nd4jLong* outputShape;

            int width;
            int height;
            if (block.width() > 1) {
                auto newImageSize = INPUT_VARIABLE(1);
                REQUIRE_TRUE(newImageSize->lengthOf() == 2, 0, "resize_linear: Resize params is a pair of values, not %i.", newImageSize->lengthOf());
                REQUIRE_TRUE(block.numI() == 0, 0, "resize_linear: Resize params already given by the second param. Int params are expensive.");
                width = int(newImageSize->getScalar(0));
                height = int(newImageSize->getScalar(1));
            }
            else {
                REQUIRE_TRUE(block.numI() == 2, 0, "resize_linear: Neither resize width nor height are provided.");
                width = INT_ARG(0);
                height = INT_ARG(1);
            }
            
            ALLOCATE(outputShape, block.getWorkspace(), shape::shapeInfoLength(4), Nd4jLong);
            outputShape[1] = in[1];
            outputShape[2] = width;
            outputShape[3] = height;
            outputShape[4] = in[4];
            shape::updateStrides(outputShape, shape::order(in));

            shapeList->push_back(outputShape); 
            return shapeList;
        }

    }
}

#endif