//
// Created by croissant on 30/08/2022.
//

#ifndef ENGINETEST_SNAKE_H
#define ENGINETEST_SNAKE_H

#include <list>

using uint = unsigned int;

struct point {
	int x, y;
};


enum direction {
	LEFT, RIGHT, UP, DOWN
};

enum cell {
	EMPTY, SNAKE, FRUIT
};

class Snake {
	//body, first one is head
	std::list<point> segments;
	direction dir = DOWN;

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

		for (auto it = segments.rbegin(); it != segments.rend()++; ++it) {
			*it = *std::next(it);
		}
		segments.front() = next;
	}

	void turn(direction d) {
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
};

class SnakeGame final {
	uint width, height;
	cell* board;
	Snake snake;
public:

	bool running = false;
	struct R {
		uint width, height;
		Snake::R snake;
	};

	SnakeGame(uint width, uint height)
			: width(width), height(height),
			  snake({(int) width / 2, (int) height / 2}) {
		board = new cell[width * height];
		auto head = snake.head();
		board[head.y * width + head.x] = SNAKE;
	}

	void start() {
		running = true;
	}

	bool step() {
		auto old_tail = snake.tail();
		snake.move();
		auto head = snake.head();
		auto tip = head.x + head.y * width;
		if (0 <= head.x && head.x < width && 0 <= head.y && head.y < height) {
			if (board[tip] == SNAKE) return false;
			if (board[tip] == FRUIT) {
				snake.grow(old_tail);
			} else board[tip] = EMPTY;
			board[tip] = SNAKE;
			return true;
		} else return false;
	}

	R getR() {
		return R{width, height, snake.getR()};
	}

	~SnakeGame() {
		delete[] board;
	}
};

#endif //ENGINETEST_SNAKE_H
