#include <bits/stdc++.h>
#include <optional>
using namespace std;

class Person{
    private:
        string name;
        int age;
        string gender;
    public:
        Person(string name, int age, string gender) : name(name
), age(age), gender(gender) {}
        string getName() const{
            return name;
        }
        int getAge() const{
            return age;
        }
        string getGender() const{
            return gender;
}
};

enum class PlayerType{
    BATSMAN,
    BOWLER,
    ALLROUNDER,
    CAPTAIN,
    WICKETKEEPER
};

enum class WicketType{
    BOWLED,
    CAUGHT,
    LBW,
    RUNOUT,
    STUMPED
};

enum class BallType{
    NORMAL,
    WIDE,
    NOBALL,
    BYE,
    LEG_BYE
};

enum class RunType{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    SIX
};

class BallDetails;
class OverDetails;
class Team;
class Player;
class Wicket;

class ScoreUpdaterObserver{
    public:
        virtual ~ScoreUpdaterObserver() = default;
        virtual void updateScore(BallDetails& ballDetails) = 0;
};

class BatsmanScoreUpdater : public ScoreUpdaterObserver{
    public:
        void updateScore(BallDetails& ballDetails) override;
};

class BowlerScoreUpdater : public ScoreUpdaterObserver{
    public:
        void updateScore(BallDetails& ballDetails) override;
};

class Wicket{
    private:
        WicketType wicketType = WicketType::RUNOUT;
        shared_ptr<Player> bowler;
        OverDetails* overDetails = nullptr;
        BallDetails* ballDetails = nullptr;
    public:
        Wicket() = default;
        Wicket(WicketType type, Player bowler, OverDetails overDetails, BallDetails ballDetails);
        WicketType getWicketType() const{
            return wicketType;
        }
};

class BallDetails{
    private:
        int ballNumber = 0;
        BallType ballType = BallType::NORMAL;
        RunType runType = RunType::ZERO;
        Player* batsman = nullptr;
        Player* bowler = nullptr;
        shared_ptr<Wicket> wicket = make_shared<Wicket>();
        vector<shared_ptr<ScoreUpdaterObserver>> scoreObservers;
    public:
        BallDetails(int ballNumber, BallType ballType, RunType runType, Player batsman, Player bowler, Wicket wicket);
        explicit BallDetails(int ballNumber);
        void startBallDelivery(Team* battingTeam, Team* bowlingTeam, OverDetails overDetails);
        void notifyScoreObservers();
        BallType getBallType() const;
        const Wicket& getWicket() const;
        RunType getRunType() const;
        Player& getBatsman() const;
        Player& getBowler() const;
        bool isWicketTaken() const;
};

class OverDetails{
    private:
        int overNumber;
        vector<BallDetails> balls;
        int extraBalls;
        shared_ptr<Player> bowler;
    public:
        OverDetails(int overNumber, Player bowler);
        bool startOver(Team* battingTeam, Team* bowlingTeam, int runstoWin);
        Player& getBowler() const;
};

class BattingScoreCard{
    private:
        int runs;
        int ballsFaced;
        int fours;
        int sixes;
        bool isOut;
        Wicket wicketDetails;
    public:
        BattingScoreCard() : runs(0), ballsFaced(0), fours(0), sixes(0), isOut(false), wicketDetails() {}
        BattingScoreCard(int runs, int ballsFaced, int fours, int sixes, bool isOut, Wicket wicketDetails) :
            runs(runs), ballsFaced(ballsFaced), fours(fours), sixes(sixes), isOut(isOut), wicketDetails(wicketDetails) {}
        void setRuns(int runs){
            this->runs = runs;
        }
        void setBallsFaced(int ballsFaced){
            this->ballsFaced = ballsFaced;
        }
        void setFours(int fours){
            this->fours = fours;
        }
        void setSixes(int sixes){
            this->sixes = sixes;
        }
        void setIsOut(bool isOut){
            this->isOut = isOut;
        }
        void setWicketDetails(Wicket wicketDetails){
            this->wicketDetails = wicketDetails;
        }
        int getRuns() const{
            return runs;
        }
        int getBallsFaced() const{      
            return ballsFaced;
        }
        int getFours() const{
            return fours;
        }
        int getSixes() const{
            return sixes;
        }
        bool getIsOut() const{
            return isOut;
        }
        Wicket getWicketDetails() const{
            return wicketDetails;
        }
};

class BowlingScoreCard{
    private:
        int overs;
        int runsGiven;
        int wicketsTaken;
        int noBalls;
        int wideBalls;
        double economyRate;
    public:
        BowlingScoreCard() : overs(0), runsGiven(0), wicketsTaken(0), noBalls(0), wideBalls(0), economyRate(0.0) {}
        BowlingScoreCard(int overs, int runsGiven, int wicketsTaken, int noBalls, int wideBalls, double economyRate) :
            overs(overs), runsGiven(runsGiven), wicketsTaken(wicketsTaken), noBalls(noBalls), wideBalls(wideBalls), economyRate(economyRate) {}
        void setOvers(int overs){
            this->overs = overs;
        }
        void setRunsGiven(int runsGiven){
            this->runsGiven = runsGiven;
        }
        void setWicketsTaken(int wicketsTaken){
            this->wicketsTaken = wicketsTaken;
        }
        void setNoBalls(int noBalls){
            this->noBalls = noBalls;
        }
        void setWideBalls(int wideBalls){
            this->wideBalls = wideBalls;
        }
        void setEconomyRate(double economyRate){
            this->economyRate = economyRate;
        }
        int getOvers() const{
            return overs;
        }
        int getRunsGiven() const{
            return runsGiven;
        }
        int getWicketsTaken() const{
            return wicketsTaken;
        }
        int getNoBalls() const{
            return noBalls;
        }
        int getWideBalls() const{
            return wideBalls;
        }
        double getEconomyRate() const{
            return economyRate;
        }
};

class Player{
    private:
        Person person;
        PlayerType playerType;
        BattingScoreCard battingScoreCard;
        BowlingScoreCard bowlingScoreCard;
    public:
        Player() = default;
        Player(Person person, PlayerType type, BattingScoreCard batScoreCard, BowlingScoreCard bowlScoreCard) :
            person(person), playerType(type), battingScoreCard(batScoreCard), bowlingScoreCard(bowlScoreCard) {}
        Person getPerson() const{
            return person;  
        }
        void printBattingScoreCard() const{
             cout << person.getName() << " - Runs: " << battingScoreCard.getRuns()
         << ", Balls: " << battingScoreCard.getBallsFaced()
         << ", Fours: " << battingScoreCard.getFours()
         << ", Sixes: " << battingScoreCard.getSixes()
         << ", Out: " << (battingScoreCard.getIsOut() ? "Yes" : "No") << endl;
        }
        void printBowlingScoreCard() const{
            cout << person.getName() << " - Overs: " << bowlingScoreCard.getOvers()
         << ", Runs Given: " << bowlingScoreCard.getRunsGiven()
         << ", Wickets: " << bowlingScoreCard.getWicketsTaken()
         << ", Economy: " << bowlingScoreCard.getEconomyRate() << endl;
        }
        BattingScoreCard& getBattingScoreCard(){
            return battingScoreCard;
        }
        const BattingScoreCard& getBattingScoreCard() const{
            return battingScoreCard;
        }
        BowlingScoreCard& getBowlingScoreCard(){
            return bowlingScoreCard;
        }
        const BowlingScoreCard& getBowlingScoreCard() const{
            return bowlingScoreCard;
        }
        string getName() const{
            return person.getName();
        }
};

class PlayerBattingController{
    private:
        deque<Player> players;
        size_t nextPlayer = 0;
        Player* striker;
        Player* nonStriker;
    public:
        PlayerBattingController(queue<Player> playingEleven){
            while(!playingEleven.empty()){
                players.push_back(playingEleven.front());
                playingEleven.pop();
            }
            striker = nullptr;
            nonStriker = nullptr;
        }
        void setStriker(Player* striker){
            this->striker = striker;
        }
        void setNonStriker(Player* nonStriker){
            this->nonStriker = nonStriker;
        }
        void swapBatsmen(){
            swap(striker, nonStriker);
        }
        void replaceStriker(){
            if(nextPlayer >= players.size()){
                throw runtime_error("No more players left to play");
            }
            striker = &players[nextPlayer++];
        }
        void getNextPlayer(){
            if(nextPlayer >= players.size()){
                throw runtime_error("No more players left to play");
            }
           if(striker == nullptr){
                striker = &players[nextPlayer++];
            } else if(nonStriker == nullptr){
                nonStriker = &players[nextPlayer++];
            } else {
                throw runtime_error("Both striker and non-striker are already set");
            }
        }

        Player* getStriker() const{
            return striker;
        }
        Player* getNonStriker() const{
            return nonStriker;
        }
        const deque<Player>& getPlayers() const{
            return players;
        }
};

class PlayerBowlerController{
    private:
        deque<Player> bowlers;
        Player* currentBowler;
        unordered_map<string, int> bowlerOverCount;
    public:
        PlayerBowlerController(vector<Player> bowlers, unordered_map<string, int> bowlerOverCount) : bowlerOverCount(bowlerOverCount){
            setBowlerList(bowlers);
            currentBowler = nullptr;
        }
        void setBowlerList(vector<Player> bowlers){
            for(auto& bowler : bowlers){
                this->bowlers.push_back(bowler);
                bowlerOverCount[bowler.getName()] = 0;
            }
        }
        void getNextBowler(int maxOverCountPerBowler){
            if(bowlers.empty()){
                throw runtime_error("No more bowlers left to bowl");
            }
            Player& nextBowler = bowlers.front();
            if(bowlerOverCount[nextBowler.getName()] >= maxOverCountPerBowler){
                bowlers.pop_front();
                getNextBowler(maxOverCountPerBowler);
            } else {
                currentBowler = &nextBowler;
                bowlerOverCount[nextBowler.getName()]++;
            }
        }
        Player* getCurrentBowler() const{
            return currentBowler;
        }
        const deque<Player>& getBowlers() const{
            return bowlers;
        }
};

class Team{
    private:
        string teamName;
        queue<Player> playingEleven;
        vector<Player> bench;
        PlayerBattingController battingController;
        PlayerBowlerController bowlerController;
        bool isWinner;
    public:
        Team() = default;
        Team(string name, queue<Player> playing11, vector<Player> bench, PlayerBattingController batCon, PlayerBowlerController bowCont) :
            teamName(name), playingEleven(playing11), bench(bench), battingController(batCon), bowlerController(bowCont) {}
        string getTeamName() const{
            return teamName;
        }
        void chooseNextBatsman(){
            battingController.getNextPlayer();
        }
        void chooseNextBowler(int maxOverCountPerBowler){
            bowlerController.getNextBowler(maxOverCountPerBowler);
        }
           Player& getStrikerBatsman(){
               return *battingController.getStriker();
        }
           Player& getNonStrikerBatsman(){
             return *battingController.getNonStriker();
        }
        Player getBowler(){
            return *bowlerController.getCurrentBowler();
        }
           void setStrikerBatsman(Player& player){
               battingController.setStriker(&player);
        }
           void setNonStrikerBatsman(Player& player){
               battingController.setNonStriker(&player);
           }
           void swapBatsmen(){
               battingController.swapBatsmen();
        }
           void replaceStrikerBatsman(){
               battingController.replaceStriker();
           }
        void setWinner(bool winner){
            isWinner = winner;
        }
        void printBattingScoreCard(){
            for(const Player& player : battingController.getPlayers()){
                player.printBattingScoreCard();
            }
        }
        void printBowlingScoreCard(){
            for(const Player& player : bowlerController.getBowlers()){
                player.printBowlingScoreCard();
            }
        }
        int getTotalRuns(){
             int totalRuns = 0;
            for(const Player& player : battingController.getPlayers()){
                totalRuns += player.getBattingScoreCard().getRuns();
            }
            return totalRuns;
        }
};

OverDetails::OverDetails(int overNumber, Player bowler) :
    overNumber(overNumber),
    extraBalls(0),
    bowler(make_shared<Player>(move(bowler))) {}

Wicket::Wicket(WicketType type, Player bowler, OverDetails overDetails, BallDetails ballDetails) :
    wicketType(type),
    bowler(make_shared<Player>(move(bowler))),
    overDetails(&overDetails),
    ballDetails(&ballDetails) {}

BallDetails::BallDetails(int ballNumber, BallType ballType, RunType runType,
                         Player batsman, Player bowler, Wicket wicket) :
    ballNumber(ballNumber),
    ballType(ballType),
    runType(runType),
    batsman(&batsman),
    bowler(&bowler),
    wicket(make_shared<Wicket>(move(wicket))) {}

BallDetails::BallDetails(int ballNumber) : ballNumber(ballNumber) {
    scoreObservers.push_back(make_shared<BatsmanScoreUpdater>());
    scoreObservers.push_back(make_shared<BowlerScoreUpdater>());
}

void BallDetails::startBallDelivery(Team* battingTeam, Team* bowlingTeam, OverDetails overDetails) {
    batsman = &battingTeam->getStrikerBatsman();
    bowler = &overDetails.getBowler();
    ballType = BallType::NORMAL;
    if(isWicketTaken()) {
        runType = RunType::ZERO;
        wicket = make_shared<Wicket>(WicketType::BOWLED, bowlingTeam->getBowler(), overDetails, *this);
        battingTeam->replaceStrikerBatsman();
    } else {
        runType = getRunType();
        if(runType == RunType::ONE || runType == RunType::THREE) {
            battingTeam->swapBatsmen();
        }
    }
    notifyScoreObservers();
}

void BallDetails::notifyScoreObservers() {
    for(const auto& observer : scoreObservers) {
        observer->updateScore(*this);
    }
}

BallType BallDetails::getBallType() const { return ballType; }
const Wicket& BallDetails::getWicket() const { return *wicket; }
RunType BallDetails::getRunType() const {
    return static_cast<double>(rand()) / RAND_MAX < 0.5
        ? RunType::ZERO
        : RunType::ONE;
}
Player& BallDetails::getBatsman() const { return *batsman; }
Player& BallDetails::getBowler() const { return *bowler; }
bool BallDetails::isWicketTaken() const {
    return static_cast<double>(rand()) / RAND_MAX < 0.1;
}

Player& OverDetails::getBowler() const { return *bowler; }

bool OverDetails::startOver(Team* battingTeam, Team* bowlingTeam, int runstoWin) {
    int ballCount = 1;
    while(ballCount <= 6) {
        BallDetails ball(ballCount);
        ball.startBallDelivery(battingTeam, bowlingTeam, *this);
        if(ball.getBallType() == BallType::NORMAL) {
            balls.push_back(ball);
            ballCount++;
            if(ball.getWicket().getWicketType() != WicketType::RUNOUT) {
            }
            if(runstoWin <= 0 && battingTeam->getTotalRuns() > bowlingTeam->getTotalRuns()) {
                battingTeam->setWinner(true);
                return true;
            }
        } else {
            extraBalls++;
        }
    }
    return false;
}

void BatsmanScoreUpdater::updateScore(BallDetails& ballDetails) {
    int run = 0;
    switch(ballDetails.getRunType()) {
        case RunType::ONE: run = 1; break;
        case RunType::TWO: run = 2; break;
        case RunType::THREE: run = 3; break;
        case RunType::FOUR: run = 4; break;
        case RunType::SIX:
            run = 6;
            ballDetails.getBatsman().getBattingScoreCard().setSixes(
                ballDetails.getBatsman().getBattingScoreCard().getSixes() + 1);
            break;
        case RunType::ZERO: break;
    }
    auto& scoreCard = ballDetails.getBatsman().getBattingScoreCard();
    scoreCard.setRuns(scoreCard.getRuns() + run);
    scoreCard.setBallsFaced(scoreCard.getBallsFaced() + 1);
    if(ballDetails.getWicket().getWicketType() != WicketType::RUNOUT) {
        scoreCard.setIsOut(true);
        scoreCard.setWicketDetails(ballDetails.getWicket());
    }
}

void BowlerScoreUpdater::updateScore(BallDetails& ballDetails) {
    int run = 0;
    switch(ballDetails.getRunType()) {
        case RunType::ONE: run = 1; break;
        case RunType::TWO: run = 2; break;
        case RunType::THREE: run = 3; break;
        case RunType::FOUR: run = 4; break;
        case RunType::SIX: run = 6; break;
        case RunType::ZERO: break;
    }
    auto& scoreCard = ballDetails.getBowler().getBowlingScoreCard();
    scoreCard.setRunsGiven(scoreCard.getRunsGiven() + run);
    if(ballDetails.getWicket().getWicketType() != WicketType::RUNOUT) {
        scoreCard.setWicketsTaken(scoreCard.getWicketsTaken() + 1);
    }
}

class MatchType{
    public:
        virtual ~MatchType() = default;
        virtual int noOfOvers() const = 0;
        virtual int maxOverCountPerBowler() const = 0;
};

class OneDayMatch : public MatchType {
public:
    int noOfOvers() const override {
        return 50;
    }
    int maxOverCountPerBowler() const override {
        return 10;
    }
};

class T20Match : public MatchType {
public:
    int noOfOvers() const override {
        return 20;
    }
    int maxOverCountPerBowler() const override {
        return 4;
    }
};

class InningDetails{
    private:
        Team* battingTeam;
        Team* bowlingTeam;
        MatchType* matchType;
        vector<OverDetails> overs;
    public:
        InningDetails() = default;
        InningDetails(Team* battingTeam, Team* bowlingTeam, MatchType* matchType) : battingTeam(battingTeam), bowlingTeam(bowlingTeam), matchType(matchType) {}
        void start(int runsToWin){
            try{
                battingTeam->chooseNextBatsman();
                battingTeam->chooseNextBatsman();
            }
            catch(const runtime_error& e){
                cout << "Error: " << e.what() << endl;
                return;
            }
            int totalOvers = matchType->noOfOvers();
            for(int i = 1; i <= totalOvers; i++){
                bowlingTeam->chooseNextBowler(matchType->maxOverCountPerBowler());
                OverDetails over(i, bowlingTeam->getBowler());
                overs.push_back(over);
                try{
                    bool won = over.startOver(battingTeam, bowlingTeam, runsToWin);
                    if(won){
                        break;
                    }
                }
                catch(const runtime_error& e){
                    cout << "Error: " << e.what() << endl;
                    break;
                }
                battingTeam->swapBatsmen();
            }
        }
    int getTotalRunsScored(){
            return battingTeam->getTotalRuns();
        }
};

class Match{
    private:
        Team teamA;
        Team teamB;
        string date;
        string venue;
        Team tossWinner;
        vector<InningDetails> inningDetails;
        MatchType* matchType;
    public:
        Match(Team teamA, Team teamB, string date, string venue, Team tossWinner, vector<InningDetails> details, MatchType* type) :
            teamA(teamA), teamB(teamB), date(date), venue(venue), tossWinner(tossWinner), inningDetails(details), matchType(type) {}
        void startMatch(){
            tossWinner = teamA;
            for(int inning = 1 ; inning<=2 ; ++inning){
                Team* bowlingTeam;
                Team* battingTeam;
                InningDetails inningDetail;
                if(inning == 1){
                    battingTeam = &tossWinner;
                    bowlingTeam = (tossWinner.getTeamName() == teamA.getTeamName()) ? &teamB : &teamA;
                    inningDetail = InningDetails(battingTeam, bowlingTeam, matchType);
                    inningDetail.start(-1);
                }
                else{
                    bowlingTeam = &tossWinner;
                    battingTeam = (tossWinner.getTeamName() == teamA.getTeamName())
                    ? &teamB : &teamA;
                    inningDetail = InningDetails(battingTeam, bowlingTeam, matchType);
                    int runsToWin = inningDetail.getTotalRunsScored() + 1;
                    inningDetail.start(runsToWin);
                    if(battingTeam->getTotalRuns() > bowlingTeam->getTotalRuns()){
                        battingTeam->setWinner(true);
                    }
                }
                inningDetails.push_back(inningDetail);
                cout << "Inning " << inning << " completed. Total runs scored by " << battingTeam->getTeamName() << ": " << battingTeam->getTotalRuns() << endl;
                cout << "Batting Scorecard for " << battingTeam->getTeamName() << ":" << endl;
                battingTeam->printBattingScoreCard();
                cout << "Bowling Scorecard for " << bowlingTeam->getTeamName() <<
    " :" << endl;
                bowlingTeam->printBowlingScoreCard();
            }
            cout << "Match won by " << teamA.getTeamName() << endl;
        }
};

    int main() {
        queue<Player> homePlayers;
        queue<Player> awayPlayers;
        vector<Player> homeBowlers;
        vector<Player> awayBowlers;

        for(int playerNumber = 1; playerNumber <= 11; ++playerNumber) {
            Player homePlayer(
                Person("Team A Player " + to_string(playerNumber), 25, "M"),
                PlayerType::ALLROUNDER,
                BattingScoreCard(),
                BowlingScoreCard());
            Player awayPlayer(
                Person("Team B Player " + to_string(playerNumber), 25, "M"),
                PlayerType::ALLROUNDER,
                BattingScoreCard(),
                BowlingScoreCard());

            homePlayers.push(homePlayer);
            awayPlayers.push(awayPlayer);
            if(playerNumber <= 5) {
                homeBowlers.push_back(homePlayer);
                awayBowlers.push_back(awayPlayer);
            }
        }

        PlayerBattingController homeBattingController(homePlayers);
        PlayerBattingController awayBattingController(awayPlayers);
        PlayerBowlerController homeBowlerController(homeBowlers, {});
        PlayerBowlerController awayBowlerController(awayBowlers, {});

        Team homeTeam("Team A", homePlayers, {}, homeBattingController, homeBowlerController);
        Team awayTeam("Team B", awayPlayers, {}, awayBattingController, awayBowlerController);
        T20Match matchType;

        Match match(homeTeam, awayTeam, "2026-09-06", "Local Ground", homeTeam, {}, &matchType);
        match.startMatch();
        return 0;
    }