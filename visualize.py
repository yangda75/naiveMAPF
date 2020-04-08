import matplotlib.pyplot as plt
from matplotlib.patches import Circle, Rectangle
from matplotlib import animation
import json
COLORS = {'goal': 'green', 'agent': 'blue', 'obstacle': 'red'}


class Animation:
    def __init__(self, map_json, solution_json):
        self.map = map_json
        self.solution = solution_json

        aspect = self.map["dimX"] / self.map["dimY"]
        self.fig = plt.figure(frameon=False, figsize=(4 * aspect, 4))
        self.ax = self.fig.add_subplot(111, aspect='equal')
        self.fig.subplots_adjust(
            left=0, right=1, bottom=0, top=1, wspace=None, hspace=None)

        self.patches = []
        self.agents = {}
        xmin = -0.5
        ymin = -0.5
        xmax = self.map["dimX"] - 0.5
        ymax = self.map["dimY"] - 0.5

        plt.xlim(xmin, xmax)
        plt.ylim(ymin, ymax)

        self.patches.append(
            Rectangle((xmin, ymin),
                      xmax - xmin,
                      ymax - ymin,
                      facecolor="none",
                      edgecolor='red'))
        for o in map_json["obstacles"]:
            x, y = o[0], o[1]
            # red rectangle as obstacles
            self.patches.append(
                Rectangle((x - 0.5, y - 0.5),
                          1,
                          1,
                          facecolor=COLORS['obstacle'],
                          edgecolor='black'))

        self.T = 0
        for g in map_json['goals']:
            self.patches.append(
                Rectangle((g[0] - 0.4, g[1] - 0.4),
                          0.8,
                          0.8,
                          facecolor=COLORS['goal'],
                          edgecolor='black'))

        for s, i in zip(map_json['starts'], range(len(map_json['starts']))):
            self.agents[i] = Circle((s[0], s[1]),
                                    0.3,
                                    facecolor=COLORS['agent'],
                                    edgecolor='black')
            self.patches.append(self.agents[i])
            self.T = max(self.T, len(self.solution['solutions'][i]['path']))

        self.animation = animation.FuncAnimation(
            self.fig,
            self.animate_func,
            init_func=self.init_func,
            frames=int(self.T + 1) * 10,
            interval=100,
            blit=True)

    def init_func(self):
        for p in self.patches:
            self.ax.add_patch(p)
        return self.patches

    def animate_func(self, i):
        for j in range(self.map['numberofagents']):
            agent = self.solution['solutions'][j]['agent']
            pos = self.get_state(int(i / 10), agent)
            p = (pos[0], pos[1])
            self.agents[agent].center = p
        for _, a in self.agents.items():
            a.set_facecolor(COLORS['agent'])

        return self.patches

    def get_state(self, index, agent):
        if self.solution['solutions'][agent]['agent'] == agent:
            if index > len(self.solution['solutions'][agent]['path']) - 1:
                return self.solution['solutions'][agent]['path'][-1]
            else:
                return self.solution['solutions'][agent]['path'][index]
        else:
            for i in range(len(self.solution['solutions'])):
                if self.solution['solutions'][i]['agent'] == agent:
                    if index > len(
                            self.solution['solutions'][agent]['path']) - 1:
                        return self.solution['solutions'][agent]['path'][-1]
                    else:
                        return self.solution['solutions'][agent]['path'][index]
            return [-1, -1]

    def show(self):
        plt.show()


if __name__ == "__main__":
    with open("test/sampleSchedule.json") as solution_json_file:
        solution_json = json.load(solution_json_file)
    with open("test/sampleMap.json") as map_json_file:
        map_json = json.load(map_json_file)

    animation = Animation(map_json, solution_json)
    animation.show()
