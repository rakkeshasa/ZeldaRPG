# ZeldaRPG
winAPI로 만드는 젤다 모작
</BR>

간단한 소개
---
![1](https://github.com/rakkeshasa/ZeldaRPG/assets/77041622/e1ed07c7-4fc1-41fe-b889-a802f32ff9fb)
</BR></BR>
winAPI방식으로 만든 젤다 모작입니다.</BR>
플레이어가 맵을 돌아다니며 필드에 존재하는 몬스터를 잡는 RPG형식 게임입니다.
</BR></BR>

플레이 영상
---
https://github.com/rakkeshasa/ZeldaRPG/assets/77041622/d7bc2b28-4fb3-40d1-a5df-eccf4621b32f

코드 구현
---
### [플레이어 이동 및 무기 스왑]
```
void TickIdle()
{
   if (GET_SINGLE(InputManager)->GetButton(KeyType::W))
   {
      SetDir(DIR_UP);

      Vec2Int nextPos = _cellPos + deltaXY[_dir];
         if (CanGo(nextPos))
         {
            SetCellPos(nextPos);
            SetState(ObjectState::Move);
         }
   }
   else if (GET_SINGLE(InputManager)->GetButton(KeyType::S))
   {
      SetDir(DIR_DOWN);

      Vec2Int nextPos = _cellPos + deltaXY[_dir];
         if (CanGo(nextPos))
         {
            SetCellPos(nextPos);
            SetState(ObjectState::Move);
         }
   }
   else
   {
      _keyPressed = false;
         if (_state == ObjectState::Idle)
            UpdateAnimation();
   }

If (GET_SINGLE(InputManager)
   ->GetButtonDown(KeyType::KEY_1))
   {
      SetWeaponType(WeaponType::Sword);
   }
   else if (GET_SINGLE(InputManager)
      ->GetButtonDown(KeyType::KEY_2))
   {
      SetWeaponType(WeaponType::Bow);
   }
   if (GET_SINGLE(InputManager)
      ->GetButton(KeyType::SpaceBar))
   {
      SetState(ObjectState::Skill);
   }
}
```
플레이어는 W, A, S, D를 이용해 이동이 가능합니다. 해당 코드에서는 반복 되는 부분이 많아 A, D는 삭제하였습니다.
<br>W, A, S, D중 하나라도 누를 시 우선 SetDir()함수를 통해 플레이어의 방향을 돌리고, 애니메이션을 실행합니다.
<br>그 후 nextPos에 앞 칸의 좌표를 계산합니다. deltaXY는 각 방향별 바로 앞칸을 나타내는 배열입니다.
<br>CanGo()함수를 통해 앞으로 갈 수 있는지 판별합니다.
<br>갈 수 있다면 SetCellPos()를 통해 플레이어 좌표를 한 칸 앞으로 이동시킵니다.
<br>SetState()함수를 통해 move애니메이션을 실행할 수 있게 합니다.
<br>아무 방향 키도 안누르고 있다면 idle 애니메이션을 실행합니다.
<br><br>

### [몬스터 길 찾기]
```
bool FindPath()
{
   priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;
   int32 cost = abs(dest.y - src.y) + abs(dest.x - src.x);

   pq.push(PQNode(cost, src));
   best[src] = cost;
   parent[src] = src;

   while (pq.empty() == false)
   {
      PQNode node = pq.top();
      pq.pop();

      if (best[node.pos] < node.cost)   continue;
		
      if (node.pos == dest)
      {
         found = true;
         break;
      }
		
      for (int32 dir = 0; dir < 4; dir++)
      {
         Vec2Int nextPos = node.pos + front[dir];

         if (CanGo(nextPos) == false)
            continue;

         int32 depth = abs(dest.y - src.y) + abs(dest.x - src.x);
         if (depth >= maxDepth)   continue;

         int32 cost = abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x);
         if (bestCost != 0)
         {
            if (best[nextPos] <= cost)   continue;
         }

         best[nextPos] = cost;
         pq.push(PQNode(cost, nextPos));
         parent[nextPos] = node.pos;
		}
	}

	if (found == false)
	{
		// 길이 막혔다
		float bestScore = FLT_MAX;

		for (auto& item : best)
		{
			Vec2Int pos = item.first;
			int32 score = item.second;

			// 동점이라면 최초 위치에서 가장 덜 이동하는 방법
			if (bestScore == score)
			{
				int32 dist1 = abs(dest.x - src.x) + abs(dest.y - src.y);
				int32 dist2 = abs(pos.x - src.x) + abs(pos.y - src.y);
				if (dist1 > dist2)
					dest = pos;
			}
			else if (bestScore > score)
			{
				dest = pos;
				bestScore = score;
			}
		}
	}

	path.clear();
	Vec2Int pos = dest;

	while (true)
	{
		path.push_back(pos);

		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(path.begin(), path.end());
	return true;
}
```
A*알고리즘을 이용하여 몬스터가 플레이어한테 가는 경로를 구했습니다.
<br>priority_queue를 이용하였으며, 최소치를 구하기 위해 greater<PQNode>인자를 사용했습니다.
<br>일반적인 경우에는 도착칸에 플레이어가 존재하여 길이 막힙니다.
<br>해당하는 경우에는 플레이어에게 최대한 가까이 붙는 경로로 설정을 하며 동일 cost대비 최소 경로로 이동하게 했습니다.
<br><br> 

### [플레이어 공격 구현]
```
void Player::TickSkill()
{
   if (IsAnimationEnded())
   {
      if (_weaponType == WeaponType::Sword)
      {
         Creature* creature = scene->GetCreatureAt(GetFrontCellPos());
         if (creature)
         {
            scene->SpawnObject<HitEffect>(GetFrontCellPos());
            creature->OnDamaged(this);
         }
      }
      else if (_weaponType == WeaponType::Bow)
      {
         Arrow* arrow = scene->SpawnObject<Arrow>(GetFrontCellPos());
         arrow->SetDir(_dir);
         arrow->SetCreature(this);
      }
      SetState(ObjectState::Idle);
   }
}


void Arrow::TickIdle()
{
   if (CanGo(nextPos))
   {
      SetCellPos(nextPos);
      SetState(ObjectState::Move);
   }
   else
   {
      Creature* creature = scene->GetCreatureAt(nextPos);
      Monster* monster = dynamic_cast<Monster*>(creature);
      if (monster)
      {
         scene->SpawnObject<HitEffect>(nextPos);
         creature->OnDamaged(this);
      }

      scene->RemoveActor(this);
   }
}
```
플레이어가 검을 휘두를 시 검을 휘두르는 방향 앞에 크리쳐가 있는지 확인합니다.
<br>크리쳐가 있다면 히트 이펙트를 출력하고 크리쳐에게서 OnDamaged()함수를 호출합니다.
<br>활을 쏠 시 화살을 맵 상에 배치하고 방향과 쏜 사람을 set해줍니다.
<br>화살을 쏜 플레이어가 맞을 수 있으므로 다이나믹 캐스트로 monster인지 확인합니다.
<br>몬스터가 맞았다면 히트 이펙트를 출력하고 OnDamaged()함수를 호출합니다.
