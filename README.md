# Human Readable
`hr` is a small tool for processing numeric data in text files for better readability.
It reads from `stdin` and converts every number to more readable format. Non-numerics and whitespaces are left alone.

## Usage

> $ wc my_big_file.txt | hr 

> 38.5M   533M 3.43G my_big_file.txt

### Command line options
* `-s --suffix <str>` _str_ will be appended after every number
* `-t --time` every number will be interpreted as time in seconds
* `-gi --gibi` uses binary prefixes, like _kibi_, _mebi_, _gibi_
* `-g --giga` uses SI or metric prefixes, like _kilo_, _mega_, _giga_ (this is the default)
