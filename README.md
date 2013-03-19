
Poker_in_C
HOW TO RUN: (1) command-line: "make", (2) command-line: "./a.out"
SUMMARY:
  •	Final Project for C Programming
	•	Focus of the Assignment is on Programming Fundamentals, the C Language, and Data Structures
	•	HIGHLIGHTS
	◦	Use of Monte Carlo Advisor approach to indicate to user which cards are best to hold
	◦	Monte Carlo Advisor efficiently runs and evaluates 15,000 tests for all 32 possible decisions for each hand
	◦	Queue Datastructure for dealing and returning cards
-MORE ON MONTE CARLO ADVISOR: - For each hand, every possible decision (e.g. hold first card, hold second card, hold first and second card, hold all cards) are simulated with the resulting hand valued against all other hands. - There are 32 of these possible decisions - Monte Carlo Advisor runs all possible decisions 15000 times. Based off of these results the advisor determines the best possible decision the user could make and presents that decision to the user
-KNOWN LIMITATIONS - since focus of the assignment was on data structures and the MC advisor, the program does not yet allow for user betting, and does not have a robust AI for computer player decisions
-FINAL GRADE - 100%
