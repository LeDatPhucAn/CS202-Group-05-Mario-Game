#pragma once
class Score {
    private:
        static Score* instance;
        int score;
        int coins;
        int lives;

        Score() : score(0), coins(0), lives(3) {}
    public:
        Score(const Score&) = delete;
        Score& operator=(const Score&) = delete;
        static Score* getInstance();
        void addScore(int points);
        void addCoin();
        void setLives(int newLives);
        int getScore();
        int getCoins();
        int getLives();
        void reset();
        void resetGameOnly(); 
        static void destroyInstance();
};