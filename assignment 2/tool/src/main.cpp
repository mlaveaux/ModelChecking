/**
 * Copyright 2016 Algorithms for Model Checking.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <string.h>

/**
 * The main entry point for the program.
 */
int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: [--order=input|random|type1|type2] <pg input filename>" << std::endl;
        std::cin.get(); return -1;
    }
    
    for (int i=0; i<argc; ++i) {
        std::cout << argv[i] << "\n";
    }
    
    
    return 0;
}
