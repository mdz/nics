DROP SEQUENCE uid;
CREATE SEQUENCE uid START 500 MINVALUE 500;

DROP SEQUENCE game;
CREATE SEQUENCE game START 0 MINVALUE 0;

DROP TABLE users;
CREATE TABLE users (uid INTEGER PRIMARY KEY,
		   name VARCHAR(128) UNIQUE, password VARCHAR(128));

DROP TABLE ratings;
CREATE TABLE ratings (uid INTEGER REFERENCES users(uid),
		     type SMALLINT, rating SMALLINT,
		     best_rating SMALLINT, rd SMALLINT,
		     PRIMARY KEY(uid, type));

DROP TABLE logins;
CREATE TABLE logins (uid INTEGER REFERENCES users(uid),
		    timestamp DATETIME, duration INTEGER);

DROP TABLE games;
CREATE TABLE games (id INTEGER PRIMARY KEY,
		       type SMALLINT,
			 white INTEGER REFERENCES users(uid),
			 black INTEGER REFERENCES users(uid),
			  white_rating SMALLINT, black_rating SMALLINT,
			  initial_time SMALLINT, increment SMALLINT,
			  timestamp DATETIME, winner INTEGER,
			  result SMALLINT);

DROP TABLE moves_chess;
CREATE TABLE moves_chess (game_id INTEGER REFERENCES games(id),
	     		   movenum SMALLINT, delay SMALLINT,
			   start_x SMALLINT, start_y SMALLINT,
			   end_x SMALLINT, end_y SMALLINT,
			   PRIMARY KEY(game_id, movenum));
