#!/bin/bash
project_path="/media/DevHDD/C++/vhost_extractor"

if [ ! -d "$project_path/build" ]
then
  mkdir $project_path/build
fi

if [ ! -d "$project_path/bin" ]
then
  mkdir $project_path/bin
fi

cd $project_path/build

cmake ../
cmake --build .
make
cmake --install . --prefix $project_path