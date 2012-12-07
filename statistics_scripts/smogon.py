import xml.etree.ElementTree as ET

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
	with open(file_name, 'w') as f:
		for pokemon in data:
			print>>f, normalize_name(pokemon[0]), '\t', pokemon[1]

save_file('overall_output.txt', parse_file('overall_source.txt'))
save_file('lead_output.txt', parse_file('lead_source.txt'))


def load_detailed(file_name):
	lines = []
	with open(file_name) as f:
		for line in f:
			line = line.strip()
			lines += [line[1:-1].strip()]
	return lines

separator = '----------------------------------------'

def find_separators(lines):
	separators = []
	index = 0
	for line in lines:
		if line == separator:
			separators += [index]
		index += 1
	return separators

def parse_lines_by_pokemon(lines):
	separators = find_separators(lines)
	pokemon_info = []
	data_by_pokemon = []
	for index in range(0, len(separators) - 1):
		initial_separator = separators[index]
		final_separator = separators[index + 1]
		data = lines[initial_separator + 1:final_separator]
		if data != []:
			data_by_pokemon += [data]
		else:
			pokemon_info += [data_by_pokemon]
			data_by_pokemon = []
	return pokemon_info

def delimit_value_from_text(mixed_content):
	values = mixed_content.split()
	text = ''
	# recombine all values other than the final numeric value
	for value in values[0:-1]:
		if text != '':
			text += ' '
		text += value
	# remove the % sign from the end
	value = values[-1][0:-1]
	return [text] + [value]

to_singular = {
	'abilities':'ability',
	'items':'item',
	'natures':'nature',
	'ev spreads':'ev spread',
	'moves':'move',
	'teammates':'teammate'
}

def create_xml_tree(lines):
	data = parse_lines_by_pokemon(lines)
	root = ET.Element('stats')
	for pokemon_data in data:
		pokemon = ET.SubElement(root, 'pokemon')
		species = ET.SubElement(pokemon, 'species')
		species.text = pokemon_data[0][0]
		for specific in pokemon_data[1:]:
			super_tag = specific[0].lower()
			super_element = ET.SubElement(pokemon, super_tag)
			for value in specific[1:]:
				info = delimit_value_from_text(value)
				if info[0] == 'Other':
					continue
				element = ET.SubElement(super_element, to_singular[super_tag])
				name = ET.SubElement(element, 'name')
				name.text = info[0]
				probability = ET.SubElement(element, 'probability')
				probability.text = str(float(info[1]) / 100.0)
	return root

def write_xml(root, depth = 0):
	indent = '\t' * depth
	result = indent + '<' + root.tag + '>'
	if root.text != None:
		result += root.text
	else:
		result += '\n'
		for child in root:
			result += write_xml(child, depth + 1) + '\n'
		result += indent
	result += '</' + root.tag + '>'
	return result

root = create_xml_tree(load_detailed('detailed_source.txt'))
with open('output.xml', 'w') as f:
	f.write(write_xml(root))

