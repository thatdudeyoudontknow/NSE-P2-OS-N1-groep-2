#!/bin/sh
# In dit script wordt Ansible gedownload, wordt de hostfile geconfigureerd en worden de plyabooks aangeroepen

# De Hoofdserver moet geupdate/grade worden
sudo apt-get update

sudo apt-get upgrade -y
#nano installeren
sudo apt-get install nano -y

#map ansible/ansible aanmaken
sudo apt-add-repository ppa:ansible/ansible -y

# Op deze server moet ansible komen.
sudo apt-get install ansible -y

# we moeten pip installeren om de juiste python te gebruiken
sudo apt-get install pip -y

# Start the ssh-agent in the background
eval "$(ssh-agent -s)"

# Add your private key to the ssh-agent
ssh-add /home/localadmin/.ssh/id_rsa_wordpress
ssh-add /home/localadmin/.ssh/id_rsa_docker

#inhoud van de hosts
hosts_inhoud="[servers]
wordpress_host ansible_user=localhost
wordpress_host ansible_host=10.6.0.100
wordpress_host ansible_ssh_private_key_file=/home/localadmin/.ssh/id_rsa_wordpress
wordpress_host ansible_python_interpreter=/usr/bin/python3
docker_host ansible_user=localhost
docker_host ansible_host=10.6.0.136
docker_host ansible_ssh_private_key_file=/home/localadmin/.ssh/id_rsa_docker
docker_host ansible_python_interpreter=/usr/bin/python3"

# Dit is waar de hostsfile staat
hosts_bestand="/etc/ansible/hosts"

# Controleer of de gewenste inhoud al aanwezig is in de hostsfile
if sudo grep -qF "$hosts_inhoud" "$hosts_bestand"; then
    echo "De gewenste inhoud is al aanwezig in het hosts-bestand."
else
    # Voeg de gewenste inhoud toe aan het hosts-bestand met sudo-rechten
    echo "$hosts_inhoud" | sudo tee -a "$hosts_bestand" > /dev/null
    echo "Inhoud succesvol toegevoegd aan het hosts-bestand."
fi



#private key van wordpress
url_wordpress="https://github.com/thatdudeyoudontknow/NSE-P2-OS-N1-groep-2/raw/main/OS2/Linux/week%204/new_keys/pri_wordpress"

# Dit is waar de id_rsa file staat
cd /home/localadmin/.ssh || exit
wget -q "$url_wordpress" -O id_rsa_wordpress

chmod 600 .ssh/id_rsa_wordpress
echo "priv key wordpress op unix getjubet" 


#private key van docker
url_docker="https://raw.githubusercontent.com/thatdudeyoudontknow/NSE-P2-OS-N1-groep-2/main/OS2/Linux/week%204/new_keys/priv%20managed1"

# Dit is waar de id_rsa_docker file staat
cd /home/localadmin/.ssh || exit
wget -q "$url_docker" -O id_rsa_docker

chmod 600 .ssh/id_rsa_docker
echo "priv key docker op unix getjubet" 

#controle echo
echo "Inhoud van de id_rsa wordpress file is aangepast"


# de directory van de playbooks aanmaken
sudo mkdir -p /etc/ansible/ansible_quickstart

# de playbooks downloaden
url_playbookdocker="https://raw.githubusercontent.com/thatdudeyoudontknow/NSE-P2-OS-N1-groep-2/main/OS2/Linux/week%205/Playbookdocker.yaml"
url_playbookmain="https://raw.githubusercontent.com/thatdudeyoudontknow/NSE-P2-OS-N1-groep-2/main/OS2/Linux/week%205/Playbookmain.yaml"
url_playbookwordpress="https://raw.githubusercontent.com/thatdudeyoudontknow/NSE-P2-OS-N1-groep-2/main/OS2/Linux/week%205/Playbookwordpress.yaml"
# Dit is waar de playbooks komen te staan
cd /etc/ansible/ansible_quickstart || exit
sudo wget -q "$url_playbookdocker" -O Playbookdocker.yaml
sudo wget -q "$url_playbookmain" -O Playbookmain.yaml
sudo wget -q "$url_playbookwordpress" -O Playbookwordpress.yaml
echo "playbooks zijn gedownload"
echo "starten van de playbooks"
# de playbooks aanroepen
sudo ansible-playbook /etc/ansible/ansible_quickstart/Playbookdocker.yaml