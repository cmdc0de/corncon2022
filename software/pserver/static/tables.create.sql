drop table pair;

create table pair (
     pairing_id integer primary key autoincrement
   , initating_badge_id integer not null
   , initating_badge_name text not null
   , initating_random random not null
   , initating_date_time text not null
   , badge2_id integer
   , badge2_name text
   , badge2_random integer
   , badge2_date_time text 
   , pairing_code text
);

create index badge1_id on pair(initating_badge_id);
