#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include <tuple>
#include <memory>

class GUI;
class Object;

//Game Utilities
class Engine
{
public:
	Engine(std::unique_ptr<GUI>& gui,
		const std::string& levelFile,
		const std::string& playerAnimationFile,
		const std::string& EnemyAnimationFile);

	bool getGameOver() const;
	bool getGameWon() const;
	void changeGameState(Object::Command command);
	std::vector<std::unique_ptr<Object>>& getObjects();

	//Rule of 5
	~Engine();//destructor
	Engine(const Engine& src) noexcept;//copy constructor
	Engine& operator=(const Engine& src) noexcept;//copy assignment
	Engine(Engine&& src) noexcept;//move constructor
	Engine& operator=(Engine&& src) noexcept;//move assignment

private:
	bool gameOver{ false };
	bool gameWon{ false };
	std::vector<std::unique_ptr<Object>> objects;
	const std::unique_ptr<GUI>& gui;
};

#endif