from pico2d import *
#Boy state
IDLE, RUN, SLEEP = range(3)

#Boy Event
RIGHT_DOWN, LEFT_DOWN, RIGHT_UP, LEFT_UP, TIME_OUT = range(5)

key_event_table = {
    (SDL_KEYDOWN, SDLK_RIGHT) : RIGHT_DOWN,
    (SDL_KEYDOWN, SDLK_LEFT) : LEFT_DOWN,
    (SDL_KEYUP, SDLK_RIGHT) : RIGHT_UP,
    (SDL_KEYUP, SDLK_LEFT) : LEFT_UP
}

next_state_table = {
    IDLE : {RIGHT_UP : RUN, LEFT_UP : RUN, RIGHT_DOWN : RUN, LEFT_DOWN : RUN,TIME_OUT: SLEEP },
    RUN : {RIGHT_UP : IDLE, LEFT_UP : IDLE, RIGHT_DOWN : IDLE, LEFT_DOWN : IDLE},
        SLEEP: {LEFT_DOWN: RUN, RIGHT_DOWN: RUN}
}


class Boy:

    def __init__(self):
        self.event_que = []
        self.x, self.y = 800 // 2, 90
        self.image = load_image('animation_sheet.png')
        self.cur_state = IDLE
        self.dir = 1
        self.velocity = 0
        self.enter_state[IDLE](self)

    def enter_IDLE(self):
        self.timer = 50
        self.frame = 0

    def exit_IDLE(self):
        pass

    def do_IDLE(self):
        self.frame = (self.frame + 1) %8
        self.timer -=1
        if self.timer == 0:
            self.add_event(TIME_OUT)

    def draw_IDLE(self):
        if self.dir == 1:
            self.image.clip_draw(self.frame *100,300,100,100,self.x, self.y)
        else:
            self.image.clip_draw(self.frame * 100,200,100,100,self.x, self.y)

    def enter_RUN(self):
        self.frame = 0
        self.dir = self.velocity

    def exit_RUN(self):
        pass

    def do_RUN(self):
        self.frame = (self.frame + 1) % 8
        self.x += self.velocity
        self.x = clamp(25, self.x, 800-25)
        
    def draw_RUN(self):
        if self.velocity == 1:
            self.image.clip_draw(self.frame * 100, 100, 100, 100, self.x, self.y)
        else:
            self.image.clip_draw(self.frame * 100, 0, 100, 100, self.x, self.y)

    def enter_SLEEP(self):
        self.frame = 0

    def exit_SLEEP(self):
        pass

    def do_SLEEP(self):
        self.frame = (self.frame + 1) % 8

    def draw_SLEEP(self):
        if self.dir == 1:
            self.image.clip_composite_draw(self.frame*100,300,100,100,3.141592/2,'',self.x-25, self.y-25,100,100)
        else:
            self.image.clip_composite_draw(self.frame*100,200,100,100,3.141592/2,'',self.x+25, self.y-25,100,100)

    def change_state(self, state):
        self.exit_state[self.cur_state](self)
        self.enter_state[state](self)
        self.cur_state = state

    enter_state = {IDLE: enter_IDLE, RUN: enter_RUN, SLEEP: enter_SLEEP}
    exit_state = {IDLE: exit_IDLE, RUN: exit_RUN, SLEEP: exit_SLEEP}
    do_state = {IDLE: do_IDLE, RUN: do_RUN, SLEEP: do_SLEEP}
    draw_state = {IDLE: draw_IDLE, RUN: draw_RUN, SLEEP:draw_SLEEP}

    def update(self):
        self.do_state[self.cur_state](self)
        if len(self.event_que) > 0:
            event = self.event_que.pop()
            self.change_state(next_state_table[self.cur_state][event])

    def draw(self):
        self.draw_state[self.cur_state](self)

    def handle_event(self, event):
        if(event.type, event.key) in key_event_table:
            key_event = key_event_table[(event.type, event.key)]
            if key_event == RIGHT_DOWN:
                self.velocity += 1
            if key_event == LEFT_DOWN:
                self.velocity -= 1
            if key_event == RIGHT_UP:
                self.velocity -= 1
            if key_event == LEFT_UP:
                self.velocity += 1
            self.add_event(key_event)

    def add_event(self,event):
        self.event_que.append(event)















