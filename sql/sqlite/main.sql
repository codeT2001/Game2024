/*
 Navicat Premium Data Transfer

 Source Server         : mydatabase
 Source Server Type    : SQLite
 Source Server Version : 3035005
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005
 File Encoding         : 65001

 Date: 08/06/2024 04:42:31
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for applys
-- ----------------------------
DROP TABLE IF EXISTS "applys";
CREATE TABLE "applys" (
  "applyId" INTEGER NOT NULL,
  "applyName" text NOT NULL,
  "applyTime" text NOT NULL,
  PRIMARY KEY ("applyId")
);

-- ----------------------------
-- Table structure for chessgame
-- ----------------------------
DROP TABLE IF EXISTS "chessgame";
CREATE TABLE "chessgame" (
  "time" text NOT NULL,
  "step" integer NOT NULL,
  "posx" integer NOT NULL,
  "posy" integer NOT NULL,
  "color" integer NOT NULL,
  PRIMARY KEY ("time", "step")
);

PRAGMA foreign_keys = true;
