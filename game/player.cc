//
// Created by Hengruo Zhang on 2/9/20.
//

#include "game.h"

Result<vec<Object>> Player::buildDeck(const vec<pair<RSID, isize>> &v, RSID pid) {
  vec<Object> deck;
  uset<CardRegion> regions;
  umap<RSID, i32> cardCnt;
  RSID cnt = 0;
  for (pair<RSID, RSID> p : v) {
    if (p.second <= 0 || p.second > 3)
      return Result<vec<Object>>::mkErr(ErrorType::INVALID_DECK,
                                        "Invalid number of card %04d: %d.",
                                        p.first,
                                        p.second);
    cardCnt[p.first] += p.second;
    if (cardCnt[p.first] > 3)
      return Result<vec<Object>>::mkErr(ErrorType::INVALID_DECK,
                                        "Invalid number of card %04d: %d.",
                                        p.first,
                                        cardCnt[p.first]);
    for (RSID k = 0; k < p.second; k++) {
      auto res = Object::buildForDeck(rsidByTime(), p.first, pid);
      if (res.isErr())
        return res.castErr<vec<Object>>();
      auto wrapper = res.val();
      deck.push_back(wrapper);
      cnt++;
      if (cnt > DECK_SIZE)
        return Result<vec<Object>>::mkErr(ErrorType::INVALID_DECK, "Exceed deck size.");
      regions.insert(wrapper.getCard()->region);
      if (regions.size() > REGION_SIZE)
        return Result<vec<Object>>::mkErr(ErrorType::INVALID_DECK, "Exceed region size.");
    }
  }
  return Result<vec<Object>>::mkVal(deck);
}

Result<sptr<Player>> Player::build(RSID pid, vec<pair<RSID, isize>> v) {
  sptr<Player> player = make_shared<Player>();
  player->playerId = pid;
  auto res = buildDeck(std::move(v), pid);
  if (res.isErr())
    return res.castErr<sptr<Player>>();
  player->deck = res.val();
  return Result<sptr<Player>>::mkVal(player);
}

vec<RSID> Player::firstDraw() {
  vec<RSID> res;
  isize i = rand(1, DECK_SIZE) - 1;
  res.push_back(deck[i].getObjectId());
  i = rand(1, DECK_SIZE) - 1;
  res.push_back(deck[i].getObjectId());
  i = rand(1, DECK_SIZE) - 1;
  res.push_back(deck[i].getObjectId());
  i = rand(1, DECK_SIZE) - 1;
  res.push_back(deck[i].getObjectId());
  return vec<RSID>();
}
