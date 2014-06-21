#!/usr/bin/sh
cmake ../
sudo chmod -R 775 *
sudo make
sudo ctest -VV
sudo make install
cmake ../
sudo chmod -R 775 *
sudo make
sudo ctest -VV
sudo make install


