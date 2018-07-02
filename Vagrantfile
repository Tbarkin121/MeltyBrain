# -*- mode: ruby -*-
# vi: set ft=ruby :

vm_name = 'STM32F4Discovery'
Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/xenial64"
  config.vm.provision :shell, path: "bootstrap.sh"
  config.vm.network :forwarded_port, guest: 80, host: 4567

  config.vm.provider "virtualbox" do |vb|
    vb.name = vm_name

    cpus = 1;
    mem = 2048;

    if ENV['MULTICORE']
    # Autosetting CPU and mem amounts http://vl4rl.com/2014/06/04/enabling-mulitcpu-vagrant-machines/
      cpus = 2;
      host = RbConfig::CONFIG['host_os']
      if host =~ /darwin/
        mem = `sysctl -n hw.memsize`.to_i / 1024 / 1024
      elsif host =~ /linux/
        mem = `grep 'MemTotal' /proc/meminfo | sed -e 's/MemTotal://' -e 's/ kB//'`.to_i  / 1024
      elsif host =~ /mswin|mingw|cygwin/
        mem = `wmic computersystem Get TotalPhysicalMemory`.split[1].to_i / 1024 / 1024
      end
      # Use 1/4 of available memory.
      if (mem / 4) > 2048
        mem = mem / 4
      end

    end

    # Customize the amount of memory on the VM:
    vb.cpus = cpus
    vb.memory = mem

    # Pass the ST-Link debug port to the VM
    #vb.customize ['modifyvm', :id, '--usb', 'on']
    #vb.customize ['usbfilter', 'add', '0', '--target', :id, '--name', 'Segger', '--vendorid', '0x1366', '--productid', '0x0101']
    #vb.customize ['guestproperty', 'set', vm_name, '/VirtualBox/GuestAdd/VBoxService/--timesync-set-threshold', '1000']
    
    # Turn on USB 2.0 support and add usb filter to attach STLink V2 programmer.
    # Note that the VirtualBox extension pack MUST be installed first from:
    #   https://www.virtualbox.org/wiki/Downloads
    # Also on Linux be sure to add your user to the vboxusers group, see:
    #   http://unix.stackexchange.com/questions/129305/how-can-i-enable-access-to-usb-devices-within-virtualbox-guests
    vb.customize ['modifyvm', :id, '--usb', 'on']
    #vb.customize ['modifyvm', :id, '--usbehci', 'on']
    vb.customize ['usbfilter', 'add', '0', '--target', :id, '--name', 'STLink', '--vendorid', '0x0483', '--productid', '0x3748']
    # Display the VirtualBox GUI when booting the machine
    vb.gui = false

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # NOTE: This will enable public access to the opened port
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine and only allow access
  # via 127.0.0.1 to disable public access
  # config.vm.network "forwarded_port", guest: 80, host: 8080, host_ip: "127.0.0.1"

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  # config.vm.synced_folder "../data", "/vagrant_data"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  # config.vm.provider "virtualbox" do |vb|
  #   # Display the VirtualBox GUI when booting the machine
  #   vb.gui = true
  #
  #   # Customize the amount of memory on the VM:
  #   vb.memory = "1024"
  # end
  #
  # View the documentation for the provider you are using for more
  # information on available options.

  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.
  # config.vm.provision "shell", inline: <<-SHELL
  #   apt-get update
  #   apt-get install -y apache2
  # SHELL
end
end
