drop table pair;

create table pair (
     pairing_id integer primary key autoincrement
   , initating_badge_id text not null
   , initating_badge_name text not null
   , initating_random integer not null
   , initating_badge_color integer not null
   , initating_date_time text not null
   , pairing_code text not null
   , badge2_id text
   , badge2_name text
   , badge2_random integer
   , badge2_badge_color integer
   , badge2_date_time datetime
);

create index badge1_id on pair(initating_badge_id);

create unique index pcode on pair(pairing_code);

drop table registration;

create table registration (
    badge_id text not null
    , badge_name text not null
    , registration_time DATETIME not null
);

drop table high_score;

create table high_score (
    seq_id integer primary key autoincrement
    , badge_id text not null
    , badge_name text not null
    , score integer not null
    , score_create_date datetime not null
);

create index score_idx on high_score(score);

commit;
