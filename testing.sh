""
echo $'HOME'
echo $"HOME"
exit ""
echo $"$"
echo $'$'

export USER= // overflow

<< "end" cat





{
	< f < Makefile cat
	ls<dadd				##bash: f: No such file or directory


	ls | < dadd		
	< dadd | ls		##bash: dadd: No such file or directory and ls executed

	should add another flag if there is an error so i don't execute the command.
}

{
	export f = Make

	< $f'file' cat
}
 
{
	<< end cat > 1 | << end cat > 2 | << end cat > 3
	
	should add a function to generate heredoc file names so that each heredoc writes to a different file
}

{
	echo $USER'R'$USER""''""''""'$USER'$USE // cuz u have the last null var after single quotes
}