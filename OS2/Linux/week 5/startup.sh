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

# Dit moet ook geconfigureerd worden. we beginnen met de host file
#inhoud hostsfile
hosts_inhoud="[servers]\n
wordpress_host=10.6.0.100\n
wordpress_host ansible_ssh_private_key_file=~/.ssh/id_rsa_wordpress\n
wordpress_host ansible_python_interperter=/usr/bin/python3\n
docker_host=10.6.0.136\n
docker_host ansible_ssh_private_key_file=~/.ssh/id_rsa_docker\n
docker_host ansible_python_interperter=/usr/bin/python3"



# dit is waat de hostsfile staat
hosts_bestand="/etc/ansible/hosts"

# inhoud toevoegen aan hosts file met sudo rechten
echo "$hosts_inhoud" | sudo tee -a "$hosts_bestand" > /dev/null

#controle echo
echo "Inhoud van de hosts file is aangepast"


#private key van wordpress
url_wordpress="https://github.com/thatdudeyoudontknow/NSE-P2-OS-N1-groep-2/raw/main/OS2/Linux/week%204/new_keys/pri_wordpress"

# Dit is waar de id_rsa file staat
cd /home/localadmin/.ssh || exit
wget -q "$url_wordpress" -O id_rsa_wordpress

chmod 777 id_rsa_wordpress
echo "priv key wordpress op unix getjubet" 


#private key van docker
url_docker="https://github.com/thatdudeyoudontknow/NSE-P2-OS-N1-groep-2/raw/main/OS2/Linux/week%204/new_keys/pri_docker"

# Dit is waar de id_rsa_docker file staat
cd /home/localadmin/.ssh || exit
wget -q "$url_docker" -O id_rsa_docker

chmod 777 id_rsa_docker
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