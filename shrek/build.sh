PI_ADDRESS="10.44.19.23"


USER="orangepi"

ssh $USER@$PI_ADDRESS -t "mkdir -p ~/frc/shrek"
scp -r ./* $USER@$PI_ADDRESS:~/frc/shrek
ssh $USER@$PI_ADDRESS -t "cd ~/frc/shrek; chmod +x deploy/compile.sh; deploy/compile.sh; chmod +x deploy/run.sh"


