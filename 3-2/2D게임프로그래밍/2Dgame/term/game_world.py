objects = [[],[],[],[],[]]
layer_bg = 0
layer_player = 3
layer_monster = 2
layer_obstacle = 1
layer_tw = 4


def add_object(o, layer):
	objects[layer].append(o)
def remove_object(o):
	for i in range(len(objects)):
		if o in objects[i]:
			print('deleting', o)
			objects[i].remove(o)
			del o
			break
def clear():
	for o in all_objects():
		del o
	objects[0].clear()
	objects[1].clear()
	objects[2].clear()
	objects[3].clear()
	objects[4].clear()


def all_objects():
	for i in range(len(objects)):
		for o in objects[i]:
			yield o
def objects_at_layer(layer):
	for o in objects[layer]:
		yield o
def count_at_layer(layer):
	return len(objects[layer])
def update():
	for o in all_objects():
		o.update()
def draw():
	for o in all_objects():
		o.draw()

