# script to convert yaml data file into json file
# original yaml map data comes from https://github.com/whoenig/libMultiRobotPlanning
import yaml
import json
import os

def convert_yaml_into_json(yaml_file_path, json_file_path):
    with open(yaml_file_path) as fp:
        yaml_map = yaml.load(fp)
    json_map = {}
    json_map['starts'] = []
    json_map['goals'] = []
    json_map['obstacles'] = []
    json_map['numberofagents'] = 0
    for agent in yaml_map['agents']:
        json_map['numberofagents'] += 1
        json_map['goals'].append(agent['goal'])
        json_map['starts'].append(agent['start'])
    json_map['dimX'] = yaml_map['map']['dimensions'][0]
    json_map['dimY'] = yaml_map['map']['dimensions'][1]
    for o in yaml_map['map']['obstacles']:
        json_map['obstacles'].append(o)

    with open(json_file_path, 'w') as fp:
        json.dump(json_map, fp)


if __name__ == "__main__":
    directory = os.fsencode('benchmark/8x8_obst12')

    for file in os.listdir(directory):
        filename = os.fsdecode(file)
        if filename.endswith(".yaml"): 
            print(os.path.join(os.fsdecode(directory), os.fsdecode(filename)))
            newfilename = filename.replace('.yaml','.json')
            yaml_path = os.path.join(os.fsdecode(directory), os.fsdecode(filename))
            json_path = os.path.join(os.fsdecode(directory), os.fsdecode(newfilename))
            convert_yaml_into_json(yaml_path,json_path)
