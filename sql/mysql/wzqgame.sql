/*
 Navicat Premium Data Transfer

 Source Server         : mysql_root
 Source Server Type    : MySQL
 Source Server Version : 80300
 Source Host           : localhost:3306
 Source Schema         : wzqgame

 Target Server Type    : MySQL
 Target Server Version : 80300
 File Encoding         : 65001

 Date: 08/06/2024 04:38:38
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for applys
-- ----------------------------
DROP TABLE IF EXISTS `applys`;
CREATE TABLE `applys`  (
  `id` int NOT NULL,
  `applyid` int NOT NULL,
  `applyName` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `applyTime` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`id`, `applyid`) USING BTREE,
  INDEX `id2`(`applyid` ASC) USING BTREE,
  CONSTRAINT `applys_ibfk_1` FOREIGN KEY (`id`) REFERENCES `player` (`uid`) ON DELETE CASCADE ON UPDATE RESTRICT,
  CONSTRAINT `applys_ibfk_2` FOREIGN KEY (`applyid`) REFERENCES `player` (`uid`) ON DELETE CASCADE ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for friends
-- ----------------------------
DROP TABLE IF EXISTS `friends`;
CREATE TABLE `friends`  (
  `id` int NOT NULL,
  `fid` int NOT NULL,
  PRIMARY KEY (`id`, `fid`) USING BTREE,
  INDEX `id2`(`fid` ASC) USING BTREE,
  CONSTRAINT `friends_ibfk_1` FOREIGN KEY (`id`) REFERENCES `player` (`uid`) ON DELETE CASCADE ON UPDATE RESTRICT,
  CONSTRAINT `friends_ibfk_2` FOREIGN KEY (`fid`) REFERENCES `player` (`uid`) ON DELETE CASCADE ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for player
-- ----------------------------
DROP TABLE IF EXISTS `player`;
CREATE TABLE `player`  (
  `uid` int NOT NULL AUTO_INCREMENT,
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `email` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `pwd` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`uid`) USING BTREE,
  UNIQUE INDEX `email`(`email` ASC) USING BTREE,
  UNIQUE INDEX `name`(`name` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 11 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

SET FOREIGN_KEY_CHECKS = 1;
