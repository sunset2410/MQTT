sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa -y
sudo apt-get update
sudo apt-get install mosquitto -y

cd cJSON
sudo make
sudo make install
sudo ldconfig

cd ../mosquitto

sudo make
sudo make install
sudo ldconfig