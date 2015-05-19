#include "RoleCard.h"

RoleCard::RoleCard(string original_name, string name, int count) : Card(original_name, name, count) { }

const string RoleCard::SHERIFF = "Sceriffo";
const string RoleCard::DEPUTY = "Vice";
const string RoleCard::OUTLAW = "Fuorilegge";
const string RoleCard::RENEGATE = "Rinnegato";
