# cliclocker
#### A fork of a simple CLI clock

### Description:

###### This is a simple, 24 hours format clock that runs in a Command Line Interface (CLI).
###### You can change the foreground color by using -c or --color as an argument.
###### You can view the help menu by using -h or --help as an argument.
###### Use 'q' to quit the program.

      Usage: cliclocker [arguments] [color]
      
      Arguments:
      '-h' or '--help':             Show the help menu
      '-c' ou '--color':            Change foreground color
      
      Colors:
      blue (default)
      black
      red
      green
      yellow
      magenta
      cyan
      white

###### Cliclocker is a fork of cliclock from Youri "nbyouri" Mouton
###### This fork adds a color selector and a help menu as an argument and may add some other things in the future.

### Install:

###### You can compile the program by following these steps in the terminal, one by one:

      git clone https://github.com/Wolfterro/cliclocker.git
      cd cliclocker/
      sudo make install
      cliclocker [arguments] [color]
