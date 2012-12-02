def parse_file(file_name):
	lines = []
	with open(file_name) as f:
		for line in f:
			line = line.strip()
			if line[0] != '|':
				continue
			tokenized = line.split('|')
			lines += [[tokenized[2], tokenized[3]]]
	lines.pop(0)
	lines = [[element.strip() for element in pokemon] for pokemon in lines]
	return lines

def normalize_name(name):
	if name == 'NidoranM':
		return 'Nidoran-M'
	elif name == 'NidoranF':
		return 'Nidoran-F'
	elif name == 'Deoxys':
		return 'Deoxys-Mediocre'
	elif name == 'Wormadam':
		return 'Wormadam-Plant'
	elif name == 'Shaymin':
		return 'Shaymin-Land'
	elif name == 'Basculin':
		return 'Basculin-Blue'
	else:
		return name

def save_file(file_name, data):
	with open(file_name, 'w+') as f:
		for pokemon in data:
			print>>f, normalize_name(pokemon[0]), '\t', pokemon[1]

save_file('overall_output.txt', parse_file('overall_source.txt'))
save_file('lead_output.txt', parse_file('lead_source.txt'))
