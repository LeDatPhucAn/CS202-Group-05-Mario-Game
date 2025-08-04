#pragma once
class Score {
    private:
        static Score* instance;
        int score;
        int coins;

        Score() : score(0), coins(0) {}
    public:
        Score(const Score&) = delete;
        Score& operator=(const Score&) = delete;
        static Score* getInstance();
        void addScore(int points);
        void addCoin();
        int getScore();
        int getCoins();
        void reset();
        static void destroyInstance();
};