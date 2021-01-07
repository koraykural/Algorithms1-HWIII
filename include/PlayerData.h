/**
 * PlayerData Class.
 */

#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <iostream>

using namespace std;

struct PlayerData
{
public:
    string team;
    int point;
    int total_point;
    int rebound;
    int total_rebound;
    int assist;
    int total_assist;

    PlayerData()
    {
    }

    PlayerData(string team, int _point, int _rebound, int _assist)
        : team(team), point(_point), total_point(_point), rebound(_rebound),
          total_rebound(_rebound), assist(_assist), total_assist(_assist)
    {
    }

    PlayerData(const PlayerData &v)
    {
        team = v.team;
        point = v.point;
        total_point = v.total_point;
        rebound = v.rebound;
        total_rebound = v.total_rebound;
        assist = v.assist;
        total_assist = v.total_assist;
    }

    /**
     * Updates player data. Increments total scores.
     * 
     * @param _point {int} Point for current season.
     * @param _assist {int} Assist for current season.
     * @param _rebound {int} Rebound for current season.
     */
    void update(int _point, int _assist, int _rebound)
    {
        total_point += _point;
        total_assist += _assist;
        total_rebound += _rebound;
        point = _point;
        assist = _assist;
        rebound = _rebound;
    }
};

#endif