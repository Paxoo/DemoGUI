package main

import (
	"fmt"
	"os"

	dem "github.com/markus-wa/demoinfocs-golang/v2/pkg/demoinfocs"
	common "github.com/markus-wa/demoinfocs-golang/v2/pkg/demoinfocs/common"
	events "github.com/markus-wa/demoinfocs-golang/v2/pkg/demoinfocs/events"
	ex "github.com/markus-wa/demoinfocs-golang/v2/examples"
    metadata "github.com/markus-wa/demoinfocs-golang/v2/pkg/demoinfocs/metadata"
)


// Run parser as follows: go run parse_demofile.go -demo /path/to/demo.dem
func main() {
	// Read in demofile
	f, err := os.Open(ex.DemoPathFromArgs())
	defer f.Close()
	checkError(err)

	// Create new demoparser
	p := dem.NewParser(f)

	// Parse demofile header
	header, err := p.ParseHeader()
	checkError(err)

	currentMap := header.MapName
    mapMetadata := metadata.MapNameToMap[currentMap]  
    
    updateRate := (int(header.FrameRate()) + 1) / 4  // frames per second / 4 -> get info every 250ms
    if updateRate >= 0 && updateRate <= 5 {
        updateRate = 32
    }
	fmt.Printf("[updaterate] [%d] \n", updateRate) 
    
	played_round := 0
    frame_count := 0
    roundStarted := 0
    freezetimeOver := false
	print_overallInfo := true
    
	p.RegisterEventHandler(func(e events.RoundStart) {
		// Parse round start events
		gs := p.GameState()
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup rounds
		if (warmup == false) && (roundStarted == 0) {
			played_round = played_round + 1
			
			// what map are we playing? who is playing?
			if (print_overallInfo == true){
				fmt.Printf("STARTINFO, %s, %s, %s \n", header.MapName, gs.TeamTerrorists().ClanName(), gs.TeamCounterTerrorists().ClanName())
				print_overallInfo = false
			}
			
			fmt.Printf("ROUND START, %d, %d \n", gs.IngameTick(), played_round)
    		roundStarted = 1
    	}
	})

	p.RegisterEventHandler(func(e events.RoundEnd) {
		/* Parse round end events
		 */
		gs := p.GameState()
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup rounds
		if (warmup == false) && (roundStarted == 1) {

			fmt.Printf("ROUND PURCHASE, %d, %d, T, %d, %d, %d, CT, %d, %d, %d \n",
				gs.IngameTick(), played_round, 
				gs.TeamTerrorists().MoneySpentTotal(),
				gs.TeamTerrorists().MoneySpentThisRound(),
				gs.TeamTerrorists().FreezeTimeEndEquipmentValue(),
				gs.TeamCounterTerrorists().MoneySpentTotal(),
				gs.TeamCounterTerrorists().MoneySpentThisRound(),
				gs.TeamCounterTerrorists().FreezeTimeEndEquipmentValue(),
			)

			switch e.Winner {
			case common.TeamTerrorists:
				// Winner's score + 1 because it hasn't actually been updated yet
				fmt.Printf("ROUND END, %d, %d, %d, %d, T, %s, %s, %d \n", gs.IngameTick(), played_round, gs.TeamTerrorists().Score()+1, gs.TeamCounterTerrorists().Score(), gs.TeamTerrorists().ClanName(), gs.TeamCounterTerrorists().ClanName(), e.Reason)
			case common.TeamCounterTerrorists:
				fmt.Printf("ROUND END, %d, %d, %d, %d, CT, %s, %s, %d \n", gs.IngameTick(), played_round, gs.TeamTerrorists().Score(), gs.TeamCounterTerrorists().Score()+1, gs.TeamCounterTerrorists().ClanName(), gs.TeamTerrorists().ClanName(), e.Reason)
			default:
				/* It is currently unknown why rounds may end as draws. Markuswa
				suggested that it may be due to match medic. [NOTE]
				*/
				fmt.Printf("ROUND END, %d, %d, DRAW \n", gs.IngameTick(), played_round)
			}
            roundStarted = 0
            freezetimeOver = false
		}
	})

	p.RegisterEventHandler(func(e events.RoundEndOfficial) {
		// Parse official round end
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup rounds
		if (warmup == false)  {
			fmt.Printf("ROUND END OFFICIAL, %d, %d \n", p.GameState().IngameTick(), played_round)
		}
	})

	p.RegisterEventHandler(func(e events.MatchStart) {
		// Parse match start events
		// 99dmg demo doesnt contain matchstart - strange
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup match starts
		if warmup == false {
			played_round = played_round + 1
			
			// what map are we playing? who is playing?
			if (print_overallInfo == true){
				fmt.Printf("STARTINFO, %s, %s, %s \n", header.MapName, p.GameState().TeamTerrorists().ClanName(), p.GameState().TeamCounterTerrorists().ClanName())
				print_overallInfo = false
			}
			
			fmt.Printf("MATCH START, %d, %d] \n", p.GameState().IngameTick(), played_round)
			roundStarted = 1
		}
	})
	
	p.RegisterEventHandler(func(e events.RoundFreezetimeEnd) {
		// Parse end of freezetime
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup rounds
		if (warmup == false) && (roundStarted == 1) {
			fmt.Printf("FREEZE END, %d, %d \n", p.GameState().IngameTick(), played_round)
			freezetimeOver = true
		}
	})
	
	p.RegisterEventHandler(func(e events.FrameDone) {
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup rounds
		if (warmup == false) && (roundStarted == 1) {
    		if frame_count == updateRate {
    			// nade info
        		var nadeID int64 = 0
    			var nadePosXViz float64 = 0.0
    			var nadePosYViz float64 = 0.0
    			
    			// player info
    			var playerSideString string = "NA"
    			
        		gameTick := p.GameState().IngameTick()
            
                if freezetimeOver == true {
                    // get player Position
                    for _, player := range p.GameState().Participants().Playing() {
                        if player.IsAlive() == true {                   
                            playerPosXViz, playerPosYViz := mapMetadata.TranslateScale(player.Position().X, player.Position().Y) 
                            if player.Team == 2 {
            					playerSideString = "T"
            				} else if player.Team == 3 {
            					playerSideString = "CT"
            				}
                            
                            playerID := player.SteamID64
                            playerName := player.Name
                            playerXView := player.ViewDirectionX()
                            playerMoney := player.Money()
                            playerHealth := player.Health()
                            playerArmor := player.Armor()
							playerHelmet := player.HasHelmet()
							playerKit := player.HasDefuseKit()
                            playerWeapon := player.ActiveWeapon()
                            
                            fmt.Printf("PLAYER INFO, %d, %d, %d, %f, %f, %s, %s, %f, %d, %d, %d, %t, %t, %s \n", 
                                gameTick, played_round,
                                playerID, playerPosXViz, playerPosYViz, playerName, playerSideString, playerXView,
                                playerMoney, playerHealth, playerArmor, playerHelmet, playerKit,  playerWeapon)
                        }
                    }
                }
                
                // get active nades flying through the air tonight
                for _, nade := range p.GameState().GrenadeProjectiles() {
                    // ignore decoy
                    if nade.WeaponInstance.Type != 501 {
                        nadePosXViz, nadePosYViz = mapMetadata.TranslateScale(nade.Trajectory[len(nade.Trajectory)-1].X, nade.Trajectory[len(nade.Trajectory)-1].Y)
            			grenadeType := nade.WeaponInstance.Type
                		nadeID = nade.WeaponInstance.UniqueID() 
                        
                        // player
                        playerPosXViz, playerPosYViz := mapMetadata.TranslateScale(nade.Thrower.Position().X, nade.Thrower.Position().Y)
                        playerName := nade.Thrower.Name
                        if nade.Thrower.Team == 2 {
        					playerSideString = "T"
        				} else if nade.Thrower.Team == 3 {
        					playerSideString = "CT"
        				}
                        
                        playerID := nade.Thrower.SteamID64            	
                        fmt.Printf("GRENADE, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %d, %s \n",
             			    gameTick, played_round,
             			    playerID, playerPosXViz, playerPosYViz, playerName, playerSideString,
             			    nadeID, nadePosXViz, nadePosYViz, grenadeType, "air") 
     			    }
                }

        		frame_count = 0
    		}else{
                frame_count = frame_count + 1 		
    		}
		}
	})
	
	p.RegisterEventHandler(func(e events.ItemEquip) {
		// gets triggerd if someone is switching to a weapon
		// we activate this event after the freezetime, because player like to switch
		// their weapons alot during freezetime
		
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup rounds
		if (warmup == false) && (roundStarted == 1) && (freezetimeOver == true){
    		gameTick := p.GameState().IngameTick()
    		
    		var playerSideString string = "NA"
    		
    		playerPosXViz, playerPosYViz := mapMetadata.TranslateScale(e.Player.Position().X, e.Player.Position().Y) 
            if e.Player.Team == 2 {
					playerSideString = "T"
				} else if e.Player.Team == 3 {
					playerSideString = "CT"
				}
            
            playerID := e.Player.SteamID64
            playerName := e.Player.Name
            playerXView := e.Player.ViewDirectionX()
            playerMoney := e.Player.Money()
            playerHealth := e.Player.Health()
            playerArmor := e.Player.Armor()
			playerHelmet := e.Player.HasHelmet()
			playerKit := e.Player.HasDefuseKit()
            playerWeapon := e.Player.ActiveWeapon()
            
    		fmt.Printf("PLAYER INFO, %d, %d, %d, %f, %f, %s, %s, %f, %d, %d, %d, %t, %t, %s \n", 
                gameTick, played_round,
                playerID, playerPosXViz, playerPosYViz, playerName, playerSideString, playerXView,
                playerMoney, playerHealth, playerArmor, playerHelmet, playerKit, playerWeapon)
		
		}
	})
	
	p.RegisterEventHandler(func(e events.ItemPickup ) {
		// gets triggert if someone picks up a new item, either by buying or walking over it
		// thats why we ignore knife and bomb - roundstart
		
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup rounds
		if (warmup == false) && (roundStarted == 1){
    		// ignore 405 = knife, 404 = bomb
    		if (e.Weapon.Type != 405) && (e.Weapon.Type != 404) {
        		gameTick := p.GameState().IngameTick()
        		var playerSideString string = "NA" 
        		
        		if e.Player.Team == 2 {
					playerSideString = "T"
				} else if e.Player.Team == 3 {
					playerSideString = "CT"
				}
        		playerID := e.Player.SteamID64
                playerName := e.Player.Name
                playerPosXViz, playerPosYViz := mapMetadata.TranslateScale(e.Player.Position().X, e.Player.Position().Y) 
                weaponPickup := e.Weapon.Type
                
    			fmt.Printf("ITEM PICKUP, %d, %d, %d, %f, %f, %s, %s, %s \n", 
    			gameTick, played_round,
    			playerID, playerPosXViz, playerPosYViz, playerName, playerSideString, weaponPickup)
    		}
		}
	})
	
	p.RegisterEventHandler(func(e events.PlayerHurt) {
		// Parse player damage events
		
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup player hurt events
		if (warmup == false) && (roundStarted == 1) {
			// First block (game state)
			gameTick := p.GameState().IngameTick()

			// Second block (victim location)
			var victimX float64 = 0.0
			var victimY float64 = 0.0
			var VictimXViz float64 = 0.0
			var VictimYViz float64 = 0.0
			var VictimViewX float32 = 0.0

			// Third block (attacker location)
			var attackerX float64 = 0.0
			var attackerY float64 = 0.0
			var attackerXViz float64 = 0.0
			var attackerYViz float64 = 0.0
			var attackerViewX float32 = 0.0

			// Fourth block (victim player/team)
			var victimID uint64 = 0
			var victimName string = "NA"
			var victimSideString string = "NA"

			// Fifth block (attacker player/team)
			var attackerID uint64 = 0
			var attackerName string = "NA"
			var attackerSideString string = "NA"

			// Sixth block (Damage/Weapon)
			hpDmg := e.HealthDamage

			// If a player has more than 100 damage taken, squash this value back
			//down to 100. This may need to be changed in the future. [NOTE]
			if hpDmg > 100 {
				hpDmg = 100
			}
			armorDmg := e.ArmorDamage
			weaponID := e.Weapon.Type
			hitGroup := e.HitGroup

			// Find victim values
			if e.Player == nil {
				victimID = 0
			} else {
				victimID = e.Player.SteamID64
				victimX = e.Player.Position().X
				victimY = e.Player.Position().Y
				VictimXViz, VictimYViz = mapMetadata.TranslateScale(victimX, victimY)
				VictimViewX = e.Player.ViewDirectionX()
				victimName = e.Player.Name
				if e.Player.Team == 2 {
					victimSideString = "T"
				} else if e.Player.Team == 3 {
					victimSideString = "CT"
				}
			}

			// Find attacker values
			if e.Attacker == nil {
				attackerID = 0
			} else {
				attackerID = e.Attacker.SteamID64
				attackerX = e.Attacker.Position().X
				attackerY = e.Attacker.Position().Y
				attackerXViz, attackerYViz = mapMetadata.TranslateScale(attackerX, attackerY)
				attackerViewX = e.Attacker.ViewDirectionX()
				attackerName = e.Attacker.Name
				if e.Attacker.Team == 2 {
					attackerSideString = "T"
				} else if e.Attacker.Team == 3 {
					attackerSideString = "CT"
				}
			}
			
			// Print a line of the damage information
			fmt.Printf("DAMAGE, %d, %d, %d, %f, %f, %s, %s, %f, %d, %f, %f, %s, %s, %f, %d, %d, %d, %d \n",
				gameTick, played_round,
				victimID, VictimXViz, VictimYViz, victimName, victimSideString, VictimViewX,
				attackerID, attackerXViz, attackerYViz, attackerName, attackerSideString, attackerViewX,
				hpDmg, armorDmg, weaponID, hitGroup)
		}
	})

    
    p.RegisterEventHandler(func(e events.HeExplode) {
		// HE explodes

		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup grenade events
		if (warmup == false) && (roundStarted == 1) {
			gameTick := p.GameState().IngameTick()
    	    
    	    // Second block (player info)
    	    var playerID uint64 = 0
			var playerPosXViz float64 = 0.0
			var playerPosYViz float64 = 0.0
			var playerName string = "NA"
			var playerSideString string = "NA"

			// Third block (nade info)
			var nadeID int64 = 0
			var nadePosXViz float64 = 0.0
			var nadePosYViz float64 = 0.0
			
    	    // get player information
    	    if e.Thrower != nil {
    			playerName = e.Thrower.Name
    			playerPosXViz, playerPosYViz = mapMetadata.TranslateScale(e.Thrower.Position().X, e.Thrower.Position().Y)				
				if e.Thrower.Team == 2 {
					playerSideString = "T"
				} else if e.Thrower.Team == 3 {
					playerSideString = "CT"
				}
				playerID = e.Thrower.SteamID64
    	    }

    		// get nade information   	    
    	    nadePosXViz, nadePosYViz = mapMetadata.TranslateScale(e.Base().Position.X, e.Base().Position.Y)
			grenadeType := e.Base().GrenadeType
			nadeID = e.Base().Grenade.UniqueID()
            
			fmt.Printf("GRENADE, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %d, %s \n",
 			    gameTick, played_round, 
 			    playerID, playerPosXViz, playerPosYViz, playerName, playerSideString,
 			    nadeID, nadePosXViz, nadePosYViz, grenadeType, "explosion")
		}
	})
	
    p.RegisterEventHandler(func(e events.FlashExplode) {
    	// Flash explodes

		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup grenade events
		if (warmup == false) && (roundStarted == 1) {
			gameTick := p.GameState().IngameTick()
    	    
    	    // Second block (player info)
    	    var playerID uint64 = 0
			var playerPosXViz float64 = 0.0
			var playerPosYViz float64 = 0.0
			var playerName string = "NA"
			var playerSideString string = "NA"
					
			// Third block (nade info)
			var nadeID int64 = 0
			var nadePosXViz float64 = 0.0
			var nadePosYViz float64 = 0.0
			
    	    // get player information
    	    if e.Thrower != nil {
    			playerName = e.Thrower.Name
    			playerPosXViz, playerPosYViz = mapMetadata.TranslateScale(e.Thrower.Position().X, e.Thrower.Position().Y)				
				if e.Thrower.Team == 2 {
					playerSideString = "T"
				} else if e.Thrower.Team == 3 {
					playerSideString = "CT"
				}
				playerID = e.Thrower.SteamID64
    	    }

    		// get nade information   	    
    	    nadePosXViz, nadePosYViz = mapMetadata.TranslateScale(e.Base().Position.X, e.Base().Position.Y)
			grenadeType := e.Base().GrenadeType
			nadeID = e.Base().Grenade.UniqueID()
                        
			fmt.Printf("GRENADE, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %d, %s \n",
 			    gameTick, played_round,
 			    playerID, playerPosXViz, playerPosYViz, playerName, playerSideString,
 			    nadeID, nadePosXViz, nadePosYViz, grenadeType, "explosion")
		}
    })
    
    p.RegisterEventHandler(func(e events.SmokeStart) {
    	// Flash explodes

		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup grenade events
		if (warmup == false) && (roundStarted == 1) {
			gameTick := p.GameState().IngameTick()
    	    
    	    // Second block (player info)
    	    var playerID uint64 = 0
			var playerPosXViz float64 = 0.0
			var playerPosYViz float64 = 0.0
			var playerName string = "NA"
			var playerSideString string = "NA"
					
			// Third block (nade info)
			var nadeID int64 = 0
			var nadePosXViz float64 = 0.0
			var nadePosYViz float64 = 0.0
			
    	    // get player information
    	    if e.Thrower != nil {
    			playerName = e.Thrower.Name
    			playerPosXViz, playerPosYViz = mapMetadata.TranslateScale(e.Thrower.Position().X, e.Thrower.Position().Y)				
				if e.Thrower.Team == 2 {
					playerSideString = "T"
				} else if e.Thrower.Team == 3 {
					playerSideString = "CT"
				}
				playerID = e.Thrower.SteamID64
    	    }

    		// get nade information   	    
    	    nadePosXViz, nadePosYViz = mapMetadata.TranslateScale(e.Base().Position.X, e.Base().Position.Y)
			grenadeType := e.Base().GrenadeType
			nadeID = e.Base().Grenade.UniqueID()
            
			fmt.Printf("GRENADE, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %d, %s \n",
 			    gameTick, played_round,
 			    playerID, playerPosXViz, playerPosYViz, playerName, playerSideString,
 			    nadeID, nadePosXViz, nadePosYViz, grenadeType, "explosion")
		}
    })
	
	p.RegisterEventHandler(func(e events.SmokeExpired) {
    	// Flash explodes

		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup grenade events
		if (warmup == false) && (roundStarted == 1) {
			gameTick := p.GameState().IngameTick()
    	    
    	    // Second block (player info)
    	    var playerID uint64 = 0
			var playerPosXViz float64 = 0.0
			var playerPosYViz float64 = 0.0
			var playerName string = "NA"
			var playerSideString string = "NA"
					
			// Third block (nade info)
			var nadeID int64 = 0
			var nadePosXViz float64 = 0.0
			var nadePosYViz float64 = 0.0
			
    	    // get player information
    	    if e.Thrower != nil {
    			playerName = e.Thrower.Name
    			playerPosXViz, playerPosYViz = mapMetadata.TranslateScale(e.Thrower.Position().X, e.Thrower.Position().Y)				
				if e.Thrower.Team == 2 {
					playerSideString = "T"
				} else if e.Thrower.Team == 3 {
					playerSideString = "CT"
				}
				playerID = e.Thrower.SteamID64
    	    }

    		// get nade information   	    
    	    nadePosXViz, nadePosYViz = mapMetadata.TranslateScale(e.Base().Position.X, e.Base().Position.Y)
			grenadeType := e.Base().GrenadeType
			nadeID = e.Base().Grenade.UniqueID()

			fmt.Printf("GRENADE, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %d, %s \n",
 			    gameTick, played_round,
 			    playerID, playerPosXViz, playerPosYViz, playerName, playerSideString,
 			    nadeID, nadePosXViz, nadePosYViz, grenadeType, "expired")
		}
    })
    
	p.RegisterEventHandler(func(e events.GrenadeProjectileThrow) {
    	// if a player throws a nade -> its creats an entity -> event gets triggerd

		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup grenade events
		if (warmup == false) && (roundStarted == 1) {
    	    gameTick := p.GameState().IngameTick()
    	    
    	    // Second block (player info)
    	    var playerID uint64 = 0
			var playerPosXViz float64 = 0.0
			var playerPosYViz float64 = 0.0
			var playerName string = "NA"
			var playerSideString string = "NA"
			
			// Third block (nade info)
			var nadeID int64 = 0
			var nadePosXViz float64 = 0.0
			var nadePosYViz float64 = 0.0

    	    // get player information
    	    if e.Projectile.Thrower != nil {
    			playerName = e.Projectile.Thrower.Name
    			playerPosXViz, playerPosYViz = mapMetadata.TranslateScale(e.Projectile.Thrower.Position().X, e.Projectile.Thrower.Position().Y)				
				if e.Projectile.Thrower.Team == 2 {
					playerSideString = "T"
				} else if e.Projectile.Thrower.Team == 3 {
					playerSideString = "CT"
				}
				playerID = e.Projectile.Thrower.SteamID64
    	    }
    	    
    	    // get nade information   	    
    	    nadePosXViz, nadePosYViz = mapMetadata.TranslateScale(e.Projectile.Trajectory[0].X, e.Projectile.Trajectory[0].Y)
    		grenadeType := e.Projectile.WeaponInstance.Type
    		nadeID = e.Projectile.WeaponInstance.UniqueID()
    		
            fmt.Printf("GRENADE, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %d, %s \n",
 			    gameTick, played_round,
 			    playerID, playerPosXViz, playerPosYViz, playerName, playerSideString,
 			    nadeID, nadePosXViz, nadePosYViz, grenadeType, "create") 
            	
        }
	})

	p.RegisterEventHandler(func(e events.GrenadeProjectileDestroy) {
		// gets triggered if an Entitiy gets destroyed --> nade doesnt exist anymore
		// HE, Flash explosion is events.GrenadeEventIf
		// Smoke explosion and vanish is events.GrenadeEventIf

		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup grenade events
		if (warmup == false) && (roundStarted == 1) {
			// First block (game state)
			gameTick := p.GameState().IngameTick()

			// Second block (player info)
			var playerID uint64 = 0
			var playerPosXViz float64 = 0.0
			var playerPosYViz float64 = 0.0
			var playerName string = "NA"
			var playerSideString string = "NA"
			
			// Third block (nade info)
			var nadeID int64 = 0
			var nadePosXViz float64 = 0.0
			var nadePosYViz float64 = 0.0

            // get player information
			if e.Projectile.Thrower != nil {			
				playerName = e.Projectile.Thrower.Name
    			playerPosXViz, playerPosYViz = mapMetadata.TranslateScale(e.Projectile.Thrower.Position().X, e.Projectile.Thrower.Position().Y)				
				if e.Projectile.Thrower.Team == 2 {
					playerSideString = "T"
				} else if e.Projectile.Thrower.Team == 3 {
					playerSideString = "CT"
				}	
				playerID = e.Projectile.Thrower.SteamID64		
			}
				
			// get nade information   	    
    	    nadePosXViz, nadePosYViz = mapMetadata.TranslateScale(e.Projectile.Position().X, e.Projectile.Position().Y)
			grenadeType := e.Projectile.WeaponInstance.Type
            nadeID = e.Projectile.WeaponInstance.UniqueID() 		
 			if (grenadeType == 503) || (grenadeType == 502) {   					
                 fmt.Printf("GRENADE, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %d, %s \n",
     			    gameTick, played_round,
     			    playerID, playerPosXViz, playerPosYViz, playerName, playerSideString,
     			    nadeID, nadePosXViz, nadePosYViz, grenadeType, "destroy") 
 			}
		}
	})
    
    p.RegisterEventHandler(func(e events.PlayerFlashed) {
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup grenade events
		if (warmup == false) && (roundStarted == 1) {
    		var attackerID uint64 = 0
    		var attackerXViz float64 = 0.0
			var attackerYViz float64 = 0.0
			var attackerSideString string = "NA"
			
			var victimID uint64 = 0
			var victimXViz float64 = 0.0
			var victimYViz float64 = 0.0
			var victimSideString string = "NA"
			
			gameTick := p.GameState().IngameTick()

            // get attacker information
			if e.Attacker != nil {
     			attackerXViz, attackerYViz = mapMetadata.TranslateScale(e.Attacker.Position().X, e.Attacker.Position().Y)

         		if e.Attacker.Team == 2 {
     				attackerSideString = "T"
     			} else if e.Attacker.Team == 3 {
     				attackerSideString = "CT"
     			}
     			attackerID = e.Attacker.SteamID64
            }
            
            // get victim information
            if e.Player != nil {
                victimXViz, victimYViz = mapMetadata.TranslateScale(e.Player.Position().X, e.Player.Position().Y)
    
                 if e.Player.Team == 2 {
     				victimSideString = "T"
     			} else if e.Player.Team == 3 {
     				victimSideString = "CT"
     			}
     			victimID = e.Player.SteamID64
			}
			
			// did the attacker flash an enemy?
			if e.Attacker.Team != e.Player.Team {
    			
    			// we need to iterate over all playing players because observer and GOTV can be flashed aswell...
    			// we get playing Players in event roundstart
    			for _, value := range p.GameState().Participants().Playing() {
    			
        			// lets check if the player who got flashed, is not an spectator and is still alive
                    if (value.Name == e.Player.Name) &&(e.Player.IsAlive())  {
             			fmt.Printf("ENEMIESFLASHED, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %s, %s \n",
            				gameTick, played_round,
            				attackerID, attackerXViz, attackerYViz, e.Attacker.Name, attackerSideString,
            				victimID, victimXViz, victimYViz, e.Player.Name, victimSideString)
                	}
                }
    		}
    		
    		// was it a teamflash?
    		if e.Attacker.Team == e.Player.Team {
        		// we need to iterate over all playing players because observer and GOTV can be flashed aswell...
    			for _, value := range p.GameState().Participants().Playing() {
    			
        			// lets check if the player who got flashed, is not an spectator and is still alive
                    if (value.Name == e.Player.Name) &&(e.Player.IsAlive())  {
            			fmt.Printf("TEAMFLASHED, %d, %d, %d, %f, %f, %s, %s, %d, %f, %f, %s, %s \n",
            				gameTick, played_round,
            				attackerID, attackerXViz, attackerYViz, e.Attacker.Name, attackerSideString,
            				victimID, victimXViz, victimYViz, e.Player.Name, victimSideString)
                	}
                }
    		}
		}
	})
    
	p.RegisterEventHandler(func(e events.Kill) {
		// Parse player kill events
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup kill events
		if (warmup == false) && (roundStarted == 1) {
			// First block (game state)
			gameTick := p.GameState().IngameTick()

			// Second block (victim location)
			var victimX float64 = 0.0
			var victimY float64 = 0.0
			var VictimXViz float64 = 0.0
			var VictimYViz float64 = 0.0
			var VictimViewX float32 = 0.0

			// Third block (attacker location)
			var attackerX float64 = 0.0
			var attackerY float64 = 0.0
			var attackerXViz float64 = 0.0
			var attackerYViz float64 = 0.0
			var attackerAssistX float64 = 0.0
			var attackerAssistY float64 = 0.0
			var attackerAssistXViz float64 = 0.0
			var attackerAssistYViz float64 = 0.0
			var attackerViewX float32 = 0.0
			var attackerAssistViewX float32 = 0.0

			// Fourth block (victim player/team)
			var victimID uint64 = 0
			var victimName string = "NA"
			var victimSideString string = "NA"

			// Fifth block (attacker player/team)
			var attackerID uint64 = 0
			var attackerName string = "NA"
			var attackerSideString string = "NA"

			var attackerAssistID uint64 = 0
			var attackerAssistName string = "NA"
			var attackerAssistSideString string = "NA"

			// Sixth block (weapon/wallshot/headshot)
			weaponID := e.Weapon.Type
			isWallshot := e.PenetratedObjects
			isHeadshot := e.IsHeadshot
			var isFlashed bool = false

			// Find victim values
			if e.Victim == nil {
				victimID = 0
			} else {
				isFlashed = e.Victim.IsBlinded()

				victimID = e.Victim.SteamID64
				victimX = e.Victim.Position().X
				victimY = e.Victim.Position().Y
				VictimXViz, VictimYViz = mapMetadata.TranslateScale(victimX, victimY)
				VictimViewX = e.Victim.ViewDirectionX()
				victimName = e.Victim.Name
				if e.Victim.Team == 2 {
					victimSideString = "T"
				} else if e.Victim.Team == 3 {
					victimSideString = "CT"
				}
			}

			// Find attacker values
			if e.Killer == nil {
				attackerID = 0
			} else {
				attackerID = e.Killer.SteamID64
				attackerX = e.Killer.Position().X
				attackerY = e.Killer.Position().Y
				attackerXViz, attackerYViz = mapMetadata.TranslateScale(attackerX, attackerY)
				attackerViewX = e.Killer.ViewDirectionX()
				attackerName = e.Killer.Name
				if e.Killer.Team == 2 {
					attackerSideString = "T"
				} else if e.Killer.Team == 3 {
					attackerSideString = "CT"
				}
			}

			// Find assister values
			if e.Assister == nil {
				attackerAssistID = 0
			} else {
				attackerAssistID = e.Assister.SteamID64
				attackerAssistName = e.Assister.Name
				attackerAssistX = e.Assister.Position().X
				attackerAssistY = e.Assister.Position().Y
				attackerAssistXViz, attackerAssistYViz = mapMetadata.TranslateScale(attackerAssistX, attackerAssistY)
				attackerAssistViewX = e.Assister.ViewDirectionX()
				if e.Assister.Team == 2 {
					attackerAssistSideString = "T"
				} else {
					attackerAssistSideString = "CT"
				}
			}

			// Print a line of the kill information
			fmt.Printf("KILL, %d, %d, %d, %f, %f, %s, %s, %f, %d, %f, %f, %s, %s, %f, %d, %f, %f, %s, %s, %f, %d, %d, %t, %t \n",
				gameTick, played_round,
				victimID, VictimXViz, VictimYViz, victimName, victimSideString, VictimViewX,
				attackerID, attackerXViz, attackerYViz, attackerName, attackerSideString, attackerViewX,
				attackerAssistID, attackerAssistXViz, attackerAssistYViz, attackerAssistName, attackerAssistSideString, attackerAssistViewX,
				weaponID, isWallshot, isFlashed, isHeadshot)
		}
	})
	
	p.RegisterEventHandler(func(e events.BombPlanted) {
		// Parse bomb plant events
		 
		gs := p.GameState()
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup events
		if (warmup == false) && (roundStarted == 1) {
    		var playerXViz float64 = 0.0
			var playerYViz float64 = 0.0
			var playerID uint64 = 0
			var playerName string = "NA"
			var bombSite = "None"

			playerID = e.BombEvent.Player.SteamID64
			playerName = e.BombEvent.Player.Name
			playerXViz, playerYViz = mapMetadata.TranslateScale(e.BombEvent.Player.Position().X, e.BombEvent.Player.Position().Y)

			if e.Site == 65 {
				bombSite = "A"
			} else if e.Site == 66 {
				bombSite = "B"
			}
			fmt.Printf("BOMB, %d, %d, %d, %f, %f, %s, %s, %s \n",
				gs.IngameTick(), played_round,
				playerID, playerXViz, playerYViz, playerName, bombSite, "plant")
		}
	})

    p.RegisterEventHandler(func(e events.BombExplode) {
		// Parse bomb explode events

		gs := p.GameState()
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup events
		if (warmup == false) && (roundStarted == 1) {
			var playerXViz float64 = 0.0
			var playerYViz float64 = 0.0
			var playerID uint64 = 0
			var playerName string = "NA"
			var bombSite = "None"

			playerID = e.BombEvent.Player.SteamID64
			playerName = e.BombEvent.Player.Name
			playerXViz, playerYViz = mapMetadata.TranslateScale(e.BombEvent.Player.Position().X, e.BombEvent.Player.Position().Y)

			if e.Site == 65 {
				bombSite = "A"
			} else if e.Site == 66 {
				bombSite = "B"
			}
			fmt.Printf("BOMB, %d, %d, %d, %f, %f, %s, %s, %s \n",
				gs.IngameTick(), played_round,
				playerID, playerXViz, playerYViz, playerName, bombSite, "explode")
		}
	})
	
    p.RegisterEventHandler(func(e events.BombDefused) {
		// Parse bomb defuse events
		 
		gs := p.GameState()
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup events
		if (warmup == false) && (roundStarted == 1) {
			var playerXViz float64 = 0.0
			var playerYViz float64 = 0.0
			var playerID uint64 = 0
			var playerName string = "NA"
			var bombSite = "None"

			playerID = e.BombEvent.Player.SteamID64
			playerName = e.BombEvent.Player.Name
			playerXViz, playerYViz = mapMetadata.TranslateScale(e.BombEvent.Player.Position().X, e.BombEvent.Player.Position().Y)

			if e.Site == 65 {
				bombSite = "A"
			} else if e.Site == 66 {
				bombSite = "B"
			}
			fmt.Printf("BOMB, %d, %d, %d, %f, %f, %s, %s, %s \n",
				gs.IngameTick(), played_round,
				playerID, playerXViz, playerYViz, playerName, bombSite, "defuse")
		}
	})
	
	p.RegisterEventHandler(func(e events.BombPickup) {
		// Parse bomb defuse events
		 
		gs := p.GameState()
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup events
		if (warmup == false) && (roundStarted == 1) {
			var playerXViz float64 = 0.0
			var playerYViz float64 = 0.0
			var playerID uint64 = 0
			var playerName string = "NA"
			var bombSite = "NA"

			playerID = e.Player.SteamID64
			playerName = e.Player.Name
			playerXViz, playerYViz = mapMetadata.TranslateScale(e.Player.Position().X, e.Player.Position().Y)

			fmt.Printf("BOMB, %d, %d, %d, %f, %f, %s, %s, %s \n",
				gs.IngameTick(), played_round,
				playerID, playerXViz, playerYViz, playerName, bombSite, "pickup")
		}
	})
	
	p.RegisterEventHandler(func(e events.BombDropped) {
		// Parse bomb defuse events
		 
		gs := p.GameState()
		warmup := p.GameState().IsWarmupPeriod()

		// Only parse non-warmup events
		if (warmup == false) && (roundStarted == 1) {
			var playerXViz float64 = 0.0
			var playerYViz float64 = 0.0
			var playerID uint64 = 0
			var playerName string = "NA"
			var bombSite = "NA"

			playerID = e.Player.SteamID64
			playerName = e.Player.Name
			playerXViz, playerYViz = mapMetadata.TranslateScale(e.Player.Position().X, e.Player.Position().Y)

			fmt.Printf("BOMB, %d, %d, %d, %f, %f, %s, %s, %s] \n",
				gs.IngameTick(), played_round,
				playerID, playerXViz, playerYViz, playerName, bombSite, "drop")
		}
	})
	
	// Parse demofile to end
	err = p.ParseToEnd()
	checkError(err)
}

// Function to handle errors
func checkError(err error) {
	if err != nil {
		fmt.Printf("[ERROR] Demo Stream Error %s", err)
	}
}
