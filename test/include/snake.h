//
// Created by croissant on 30/08/2022.
//

#ifndef ENGINETEST_SNAKE_H
#define ENGINETEST_SNAKE_H

#include <list>
#include "IClonable.h"
#include "Object.h"

using uint = unsigned int;

struct point {
	int x, y;
};

struct segment : Amber::Object {

};

enum direction {
	LEFT, RIGHT, UP, DOWN
};

static direction opposite(direction d) {
	switch (d) {
		case LEFT:
			return RIGHT;
		case RIGHT:
			return LEFT;
		case UP:
			return DOWN;
		case DOWN:
			return UP;
	}
	return UP;
}

static bool opposite(direction d1, direction d2) {
	return d1 == opposite(d2);
}

enum cell {
	EMPTY, SNAKE, FRUIT
};

namespace SnakeEvents {
	//
	struct DirEvent {
		bool handled = false;
		const direction dir;

		DirEvent(const direction direction) : dir(direction) {}
	};

	struct CheatGrow {
		bool handled = false;
	};
}

class Snake {
	//body, first one is head
	std::list<point> segments;
	direction dir = DOWN;
	direction old = DOWN;

public:
	struct R {
		std::list<point> segments;
		direction dir;
	};

	R getR() {
		return R{segments, dir};
	}

	Snake(const point& point) {
		segments.push_back(point);
	}

	void move() {
		old = dir;
		point next = segments.front();
		switch (dir) {
			case LEFT:
				next.x -= 1;
				break;
			case RIGHT:
				next.x += 1;
				break;
			case UP:
				next.y += 1;
				break;
			case DOWN:
				next.y -= 1;
				break;
		}

		for (auto it = --segments.end(); it != segments.begin(); --it) {
			*it = *std::prev(it);
		}
		segments.front() = next;
	}

	void turn(direction d) {
		if (!opposite(old, d) && d != old)
			dir = d;
	}

	void grow(point tail) {
		segments.push_back(tail);
	}

	point head() {
		return segments.front();
	}

	point tail() {
		return segments.back();
	}

	int length() {
		return segments.size();
	}
};

class SnakeGame final {
	uint width, height;
	cell* board;
	Snake snake;
	point fruit;

	void genFruit() {
		board[fruit.x + fruit.y * height] = EMPTY;
		fruit.x = 2 + std::rand() % (width - 4);
		fruit.y = 2 + std::rand() % (height - 4);
		board[fruit.x + fruit.y * height] = FRUIT;
	}

public:

	Amber::EventManager handlers;

	bool running = false;

	struct R {
		uint width, height;
		Snake::R snake;
		point fruit;

		R(uint width, uint height, const Snake::R& snake, const point& fruit) : width(width), height(height),
		                                                                        snake(snake), fruit(fruit) {}
	};

	SnakeGame(uint width, uint height)
			: width(width), height(height),
			  board(new cell[width * height]),
			  snake({(int) width / 2, (int) height}),
			  fruit({0, 0}) {
		std::srand(time(nullptr));
		auto head = snake.head();
		board[head.y * width + head.x] = SNAKE;

		genFruit();

		handlers.addHandler(Amber::Handler<SnakeEvents::DirEvent>(
				[&](SnakeEvents::DirEvent& event) {
					snake.turn(event.dir);
				})
		);

		handlers.addHandler(Amber::Handler<SnakeEvents::CheatGrow>(
				[&](auto& e) {
					auto p = snake.tail();
					p.x += 1;
					snake.grow(p);
				}
		));

	}

	void start() {
		running = true;
//		for (int i = 0; i < 70; ++i) {
//			point p{snake.head().x, snake.head().y + i};
//			snake.grow(p);
//		}
	}

	bool step() {
		auto old_tail = snake.tail();
		snake.move();
		auto head = snake.head();
		auto tip = head.x + head.y * width;
		auto toe = old_tail.x + old_tail.y * width;
		if (0 <= head.x && head.x < width && 0 <= head.y && head.y < height) {
			if (board[tip] == SNAKE)
				return false;
			if (board[tip] == FRUIT) {
				snake.grow(old_tail);
				genFruit();
			} else board[toe] = EMPTY;
			board[tip] = SNAKE;
			return true;
		} else return false;
	}

	void update() {
		using namespace std::chrono;
		static time_point<system_clock> start = system_clock::now();
		time_point<system_clock> now = system_clock::now();
		auto ts = 0.3s;
		if (snake.length() > 10) ts = 0.2s;
		if (snake.length() > 20) ts = 0.08s;
		if (now - start >= ts) {
			running = step();
			start = now;
		}
	}

	R getR() {
		return R{width, height, snake.getR(), fruit};
	}

	~SnakeGame() {
		delete[] board;
	}
};

#endif //ENGINETEST_SNAKE_H
