//Алгоритм

#include <iostream>
#include <map>

int insertCost = 1;
int deleteCost = 1;
int replaceCost = 1;
int swapCost = 1;

size_t DLDistance(const std::string& source, const std::string& target){

    size_t N = source.length();
    size_t M = target.length();

    //Проверка крайних значений
    if (source.length() == 0) {
        return M * insertCost;
    }
    if (target.length() == 0) {
        return N * deleteCost;
    }

    
    int costMatrix[N][M];
    std::map<char, int> sourceIndexByCharacter = std::map<char, int>();
    if (source[0] != target[0]) {
        costMatrix[0][0] = std::min(replaceCost, deleteCost + insertCost);
    }
    sourceIndexByCharacter[source[0]] = 0;
    for (int i = 1; i < source.length(); i++) {
        int deleteDistance = costMatrix[i - 1][0] + deleteCost;
        int insertDistance = (i + 1) * deleteCost + insertCost;
        int matchDistance = i * deleteCost
                            + (source[i] == target[0] ? 0 : replaceCost);
        costMatrix[i][0] = std::min(std::min(deleteDistance, insertDistance),
                               matchDistance);
    }
    for (int j = 1; j < target.length(); j++) {
        int deleteDistance = (j + 1) * insertCost + deleteCost;
        int insertDistance = costMatrix[0][j - 1] + insertCost;
        int matchDistance = j * insertCost
                            + (source[0] == target[j] ? 0 : replaceCost);
        costMatrix[0][j] = std::min(std::min(deleteDistance, insertDistance),
                               matchDistance);
    }
    for (int i = 1; i < source.length(); i++) {
        int maxSourceLetterMatchIndex = source[i] == target[0] ? 0 : -1;
        for (int j = 1; j < target.length(); j++) {
            int candidateSwapIndex = sourceIndexByCharacter[target[j]];
            int jSwap = maxSourceLetterMatchIndex;
            int deleteDistance = costMatrix[i - 1][j] + deleteCost;
            int insertDistance = costMatrix[i][j - 1] + insertCost;
            int matchDistance = costMatrix[i - 1][j - 1];
            if (source[i] != target[j]) {
                matchDistance += replaceCost;
            } else {
                maxSourceLetterMatchIndex = j;
            }
            int swapDistance;
            if (jSwap != -1) {
                int iSwap = candidateSwapIndex;
                int preSwapCost;
                if (iSwap == 0 && jSwap == 0) {
                    preSwapCost = 0;
                } else {
                    preSwapCost = costMatrix[std::max(0, iSwap - 1)][std::max(0, jSwap - 1)];
                }
                swapDistance = preSwapCost + (i - iSwap - 1) * deleteCost
                               + (j - jSwap - 1) * insertCost + swapCost;
            } else {
                swapDistance = INT_MAX;
            }
            costMatrix[i][j] = std::min(std::min(std::min(deleteDistance, insertDistance), matchDistance), swapDistance);
        }
        sourceIndexByCharacter[source[i]] = i;
    }
    return costMatrix[source.length() - 1][target.length() - 1];
}

int main(){

    std::cout << DLDistance("abgcd", "bacd");
}