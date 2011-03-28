# delete the contents of output.txt
open ('output.txt', 'w').close()

import subprocess, glob, os
for i in glob.glob ('../Shoddy logs/*'):
	if not os.path.isdir (i):
	    subprocess.call (['./analog', i])
