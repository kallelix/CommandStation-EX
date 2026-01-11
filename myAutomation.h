ROSTER(3, "Default/New", "F0/F1/F2/F3/F4/F5/F6/F7")
ROSTER(10, "V100/BR112", "Licht/Rot R/Rot F//Rangiergang")
ROSTER(11, "V60/BR106", "Licht////Rangiergang")
ROSTER(12, "V180/BR118", "Licht////Rangiergang")
ROSTER(13,"BR86", "Licht////Rangiergang")
ROSTER(14,"VT135", "Licht////Rangiergang")

ALIAS(W_1, 101)
ALIAS(W_2, 102)
ALIAS(W_3, 103)
ALIAS(W_4, 104)
ALIAS(W_5, 105)
ALIAS(W_6, 106)
ALIAS(W_7, 107)
ALIAS(W_8, 108)
ALIAS(W_9, 109)
ALIAS(W_10, 110)
ALIAS(W_11, 111)
ALIAS(W_12, 112)

TURNOUTL(W_1, W_1, "W1 Einfahrt Bhf Ost")
TURNOUTL(W_2, W_2, "W2 Zufahrt Hafen")
TURNOUTL(W_3, W_3, "W3 Zufahrt Lokschuppen")
TURNOUTL(W_4, W_4, "W4 Tunnel EinfahrtWest")
TURNOUTL(W_5, W_5, "W5 Hafen Ost/Nord")
TURNOUTL(W_6, W_6, "W6 Hafen West/Nord")
TURNOUTL(W_7, W_7, "W7 DKW Hafen West")
TURNOUTL(W_8, W_8, "W8 DKW Hafen Ost")
TURNOUTL(W_9, W_9, "W9 Hafen Ost Süd")
TURNOUTL(W_10, W_10, "W10 Tunnel Nord Innen")
TURNOUTL(W_11, W_11, "W11 Tunnel Nord Aussen")
TURNOUTL(W_12, W_12, "W12 Tunnel Nord Auffahrt")

#include "myAlias.h"

AUTOSTART
    POWERON
    DELAY(1000)
    CALL(303) // Initialize signals
    CALL(305) // Init all turnouts to thrown position
DONE

SEQUENCE(303)
    CALL(SIG_SP_2_TEST)
    CALL(GBM_G2)
    CALL(GBM_G3)
    CALL(GBM_G4)
    CALL(GBM_G5)
    CALL(GBM_G6)
    CALL(GBM_G7)
    CALL(GBM_G8)
    CALL(GBM_G1)
    CALL(GBM_G11)
    CALL(GBM_G12)
    CALL(GBM_G13)
    CALL(GBM_G14)
RETURN

ROUTE(301, "Pause")
    PAUSE
DONE

ROUTE(302, "Resume")
    RESUME
DONE

ROUTE(304, "Stop all tasks")
    KILLALL
    IFNOT(GBM_G2)
        FREE(B_2)
    ENDIF
    IFNOT(GBM_G4)
        FREE(B_4)
    ENDIF
    IFNOT(GBM_G1)
        FREE(B_1)
    ENDIF
    IFNOT(GBM_G3)
        FREE(B_3)
    ENDIF
    IFNOT(GBM_G5)
        FREE(B_5)
    ENDIF
    IFNOT(GBM_G6)
        FREE(B_6)
    ENDIF
    IFNOT(GBM_G7)
        FREE(B_7)
    ENDIF
    IFNOT(GBM_G8)
        FREE(B_8)
    ENDIF
    ROUTE_INACTIVE(B_2_ROUND)
DONE

SEQUENCE(305)
    THROW(W_1)
    THROW(W_2)
    THROW(W_3)
    THROW(W_4)
    THROW(W_5)
    THROW(W_6)
    THROW(W_7)
    THROW(W_8)
    THROW(W_9)
    THROW(W_10)
    THROW(W_11)
    THROW(W_12)
DONE

ROUTE(312, "Set ABC G2")
    SET(ABC_G2)
DONE

ROUTE(313, "Reset ABC G2")
    RESET(ABC_G2)
DONE

ROUTE(306, "Set ABC G3")
    SET(ABC_G3)
DONE

ROUTE(307, "Reset ABC G3")
    RESET(ABC_G3)
DONE

ROUTE(308, "Set ABC G13")
    SET(ABC_G13)
DONE

ROUTE(309, "Reset ABC G13")
    RESET(ABC_G13)
DONE

ROUTE(310, "Set ABC G14")
    SET(ABC_G14)
DONE

ROUTE(311, "Reset ABC G14")
    RESET(ABC_G14)
DONE

AUTOMATION(312, "Stop at B_2")
    // send a loco to this automation to stop at b_2 or b_3
    AT(GBM_G2)
    PRINT("Automation Stop at B_2 triggered")
    // needed:
    // MEMORY_STORE(slot, value)
    // MEMORY_READ(slot)
    // MEMORY_CLEAR(slot)
    // CURRENTLOCO
DONE

ONTHROW(W_1)
    PRINT("W1 thrown")
    CALL(SIG_SP_2_TEST)
    RESET(ABC_G2)
    IFTHROWN(W_2)
        ACTIVATEL(SIG_SP_6)
    ENDIF
    CALL(SIG_HS_SP_1_TEST)
DONE

ONCLOSE(W_1)
    PRINT("W1 closed")
    CALL(SIG_SP_2_TEST)
    DEACTIVATEL(SIG_SP_6)
    CALL(SIG_HS_SP_1_TEST)
    SET(ABC_G2)
DONE

SEQUENCE(SIG_HS_SP_1_TEST)
    DELAY(700)
    IFTHROWN(W_1)
        DEACTIVATEL(SIG_HS_1)
    ELSE
        IF(GBM_G10)
            DEACTIVATEL(SIG_HS_1)
        ELSE
            IF(GBM_G1)
                DEACTIVATEL(SIG_SP_1)
            ELSE  
                IF(GBM_G7)
                    IF(GBM_G8)
                        ACTIVATEL(SIG_SP_1)
                    ELSE
                        ACTIVATEL(SIG_HS_1)
                    ENDIF
                ELSE
                    ACTIVATEL(SIG_HS_1)
                ENDIF
            ENDIF
        ENDIF
    ENDIF   
RETURN

ONTHROW(W_2)
    PRINT("W2 thrown")
    CALL(SIG_SP_2_TEST)
    IFTHROWN(W_1)
        ACTIVATEL(SIG_SP_6)
    ENDIF
DONE

ONCLOSE(W_2)
    PRINT("W2 closed")
    CALL(SIG_SP_2_TEST)
    DEACTIVATEL(SIG_SP_6)
DONE

SEQUENCE(SIG_SP_2_TEST)
    DELAY(700)
    IFTHROWN(W_1)
        IFCLOSED(W_2)
            ACTIVATEL(SIG_SP_2)
        ELSE
            DEACTIVATEL(SIG_SP_2)
        ENDIF
    ELSE
        DEACTIVATEL(SIG_SP_2)
    ENDIF
RETURN

ONTHROW(W_3)
    PRINT("W3 thrown")
    DEACTIVATEL(SIG_SP_3)
    IFNOT(GBM_G4)
        IFNOT(GBM_G12)
            ACTIVATEL(SIG_SP_4)
        ENDIF
    ENDIF
    UNJOIN
    LATCH(PROG_TEST)
DONE

ONCLOSE(W_3)
    PRINT("W3 closed")
    DEACTIVATEL(SIG_HS_2)
    DEACTIVATEL(SIG_SP_4)
    JOIN
    UNLATCH(PROG_TEST)
DONE

ONSENSOR(GBM_G1)
    CALL(GBM_G1)
DONE

SEQUENCE(GBM_G1)
    IF(GBM_G1)
        PRINT("Sensor GBM_G1 triggered")
    ELSE
        PRINT("Sensor GBM_G1 untriggered")
        DELAY(1000)
        IFNOT(GBM_G1)
            PRINT("Sensor GBM_G1 still untriggered, freeing B_1")
            FREE(B_1)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G11)
    CALL(GBM_G11)
DONE

SEQUENCE(GBM_G11)
    IF(GBM_G11)
        PRINT("Sensor GBM_G11 triggered")
    ELSE
        PRINT("Sensor GBM_G11 untriggered")
        DELAY(1000)
        IFNOT(GBM_G11)
            PRINT("Sensor GBM_G11 still untriggered, freeing B_11")
            FREE(B_11)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G2)
    CALL(GBM_G2)
DONE

SEQUENCE(GBM_G2)
    IF(GBM_G2)
        PRINT("Sensor GBM_G2 triggered")
        // Debounce
        DELAY(500)
        IF(GBM_G2)
            PRINT("Sensor GBM_G2 still triggered, deactivating SIG_HS_2")
            DEACTIVATEL(SIG_HS_2)
            // Better not do this here, because reservation will be anonymous then
            //IFRESERVE(B_2)
            //    PRINT("Sensor GBM_G2: Reserved B_2")
            //ENDIF
        ENDIF
    ELSE
        PRINT("Sensor GBM_G2 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G2)
            PRINT("Sensor GBM_G2 still untriggered, activating SIG_HS_2 and freeing B_2")
            IFNOT(GBM_G12)
                ACTIVATEL(SIG_HS_2)
                SET(ABC_G4)
            ENDIF
            FREE(B_2)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G3)
    CALL(GBM_G3)
DONE

SEQUENCE(GBM_G3)
    IF(GBM_G3)
        PRINT("Sensor GBM_G3 triggered")
        // Debounce
        DELAY(500)
        IF(GBM_G3)
            // Better not do this here, because reservation will be anonymous then
            //PRINT("Sensor GBM_G3 still triggered, reserving B_3")
            //IFRESERVE(B_3)
            //    PRINT("Sensor GBM_G3: Reserved B_3")
            //ENDIF
            IF(GBM_G5)
                PRINT("Sensor GBM_G3: Break to wait for B_5 to clear")
                RESET(ABC_G3)
            ENDIF
        ENDIF
    ELSE
        PRINT("Sensor GBM_G3 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G3)
            PRINT("Sensor GBM_G3 still untriggered, freeing B_3")
            FREE(B_3)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G4)
    CALL(GBM_G4)
DONE

SEQUENCE(GBM_G4)
    IF(GBM_G4)
        PRINT("Sensor GBM_G4 triggered")
        // Debounce
        DELAY(200)
        IF(GBM_G4)
            PRINT("Sensor GBM_G4 still triggered, reserving B_4")
            DEACTIVATEL(SIG_SP_4)
            // Better not do this here, because reservation will be anonymous then
            //IFRESERVE(B_4)
            //    PRINT("Sensor GBM_G4: Reserved B_4")
            //ENDIF
            IF(GBM_G2)
                IFTHROWN(W_3)
                    PRINT("Sensor GBM_G4: Break to wait for B_2 to clear")
                    RESET(ABC_G4)
                ENDIF
            ENDIF
            IF(GBM_G12)
                PRINT("Sensor GBM_G4: Break to wait for GBM_G12 to clear")
                RESET(ABC_G4)
            ENDIF
        ENDIF
    ELSE
        PRINT("Sensor GBM_G4 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G4)
            PRINT("Sensor GBM_G4 still untriggered, freeing B_4")
            IFNOT(GBM_G12)
                ACTIVATEL(SIG_SP_4)
            ENDIF
            FREE(B_4)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G12)
    CALL(GBM_G12)
DONE

SEQUENCE(GBM_G12)
    IF(GBM_G12)
        PRINT("Sensor GBM_G12 triggered")
        // Debounce
        DELAY(500)
        IF(GBM_G12)
            PRINT("Sensor GBM_G12 still triggered")
            DEACTIVATEL(SIG_HS_2)
            DEACTIVATEL(SIG_SP_4)
        ENDIF
    ELSE
        PRINT("Sensor GBM_G12 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G12)
            PRINT("Sensor GBM_G12 still untriggered")
            IFNOT(GBM_G2)
                ACTIVATEL(SIG_HS_2)
                SET(ABC_G4)
            ENDIF
            IFNOT(GBM_G4)
                ACTIVATEL(SIG_SP_4)
            ENDIF
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G5)
    CALL(GBM_G5)
DONE

SEQUENCE(GBM_G5)
    IF(GBM_G5)
        PRINT("Sensor GBM_G5 triggered")
        // Debounce
        DELAY(500)
        IF(GBM_G5)
            PRINT("Sensor GBM_G5 still triggered, deactivating SIG_HS_5")
            DEACTIVATEL(SIG_HS_5)
            // Better not do this here, because reservation will be anonymous then
            //IFRESERVE(B_5)
            //    PRINT("Sensor GBM_G5: Reserved B_5")
            //ENDIF
            IF(GBM_G6)
                PRINT("Sensor GBM_G5: Break to wait for B_6 to clear")
                RESET(ABC_G5)
            ENDIF
        ENDIF
    ELSE
        PRINT("Sensor GBM_G5 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G5)
            PRINT("Sensor GBM_G5 still untriggered, activating SIG_HS_5 and freeing B_5")
            ACTIVATEL(SIG_HS_5)
            FREE(B_5)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G6)
    CALL(GBM_G6)
DONE

SEQUENCE(GBM_G6)
    IF(GBM_G6)
        PRINT("Sensor GBM_G6 triggered")
        // Debounce
        DELAY(500)
        IF(GBM_G6)
            PRINT("Sensor GBM_G6 still triggered, deactivating SIG_VS_5, deactivating SIG_HS_6")
            DEACTIVATEL(SIG_VS_5)
            DEACTIVATEL(SIG_HS_6)
            // Better not do this here, because reservation will be anonymous then
            //IFRESERVE(B_6)
            //    PRINT("Sensor GBM_G6: Reserved B_6")
            //ENDIF
            IF(GBM_G1)
                PRINT("Sensor GBM_G6: Break to wait for B_1 to clear")
                RESET(ABC_G6)
            ENDIF
            IFTHROWN(W_10)
                PRINT("Sensor GBM_G6: Break to wait for W_10 to be closed")
                RESET(ABC_G6)
            ENDIF
        ENDIF
    ELSE
        PRINT("Sensor GBM_G6 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G6)
            PRINT("Sensor GBM_G6 still untriggered, activating SIG_VS_5, activating SIG_HS_6 and freeing B_6")
            ACTIVATEL(SIG_VS_5)
            ACTIVATEL(SIG_HS_6)
            FREE(B_6)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G7)
    CALL(GBM_G7)
DONE

SEQUENCE(GBM_G7)
    IF(GBM_G7)
        PRINT("Sensor GBM_G7 triggered")
        // Debounce
        DELAY(500)
        IF(GBM_G7)
            //PRINT("Sensor GBM_G7 still triggered, reserving B_7")
            //IFRESERVE(B_7)
            //    PRINT("Sensor GBM_G7: Reserved B_7")
            //ENDIF
        ENDIF
    ELSE
        PRINT("Sensor GBM_G7 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G7)
            //PRINT("Sensor GBM_G7 still untriggered, freeing B_7")
            //FREE(B_7)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G8)
    CALL(GBM_G8)
DONE

SEQUENCE(GBM_G8)
    IF(GBM_G8)
        PRINT("Sensor GBM_G8 triggered")
        // Debounce
        DELAY(500)
        IF(GBM_G8)
            PRINT("Sensor GBM_G8 still triggered")
            // Better not do this here, because reservation will be anonymous then
            //IFRESERVE(B_8)
            //    PRINT("Sensor GBM_G8: Reserved B_8")
            //ENDIF
        ENDIF
    ELSE
        PRINT("Sensor GBM_G8 untriggered")
        // Debounce
        DELAY(1000)
        IFNOT(GBM_G8)
            //PRINT("Sensor GBM_G8 still untriggered, freeing B_8")
            //FREE(B_8)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G13)
    CALL(GBM_G13)
DONE
SEQUENCE(GBM_G13)
    IF(GBM_G13)
        PRINT("Sensor GBM_G13 triggered")
    ELSE
        PRINT("Sensor GBM_G13 untriggered")
        DELAY(1000)
        IFNOT(GBM_G13)
            PRINT("Sensor GBM_G13 still untriggered, freeing X")
        //    FREE(B_11)
        ENDIF
    ENDIF
RETURN

ONSENSOR(GBM_G14)
    CALL(GBM_G14)
DONE
SEQUENCE(GBM_G14)
    IF(GBM_G14)
        PRINT("Sensor GBM_G14 triggered")
    ELSE
        PRINT("Sensor GBM_G14 untriggered")
        DELAY(1000)
        IFNOT(GBM_G14)
            PRINT("Sensor GBM_G14 still untriggered, freeing X")
        //    FREE(B_11)
        ENDIF
    ENDIF
RETURN
// Drive away a loco from the programming track: 989, 990, 991, 992, 993
ROUTE(989, "Drive away loco from programming track to B2")
    THROW(W_3)
    ROUTE_ACTIVE(989)
    FOLLOW(990)
DONE

SEQUENCE(990)
    IF(PROG_TEST)
        PRINT("Ready to wait for loco on prog track")
        UNJOIN
        DELAY(2000)
        // Wait for loco
        FOLLOW(991)
    ELSE
        JOIN
        DELAY(1000)
        FOLLOW(990)
    ENDIF
DONE

SEQUENCE(991)
    IF(PROG_TEST)
        READ_LOCO
        IFLOCO(-1) // failed read
            DELAY(10000)
            FOLLOW(991)
        ENDIF
        PRINT("Loco on prog track")
        ACTIVATEL(SIG_SP_3)
        DELAY(700)
        DEACTIVATEL(SIG_SP_3)
        DELAY(700)
        ACTIVATEL(SIG_SP_3)
        DELAY(700)
        DEACTIVATEL(SIG_SP_3)
        DELAY(700)
        FOLLOW(992)
    ELSE
        FOLLOW(990)
    ENDIF
DONE

SEQUENCE(992)
    PRINT("Driving loco off programming track...")
    IFRESERVE(B_4)
        PRINT("992: Instant reserved B_4")
        DELAY(1000)
    ELSE
        PRINT("992: Waiting to reserve B_4")
        RESERVE_NOESTOP(B_4)
        PRINT("992: Reserved B_4. Waiting for security delay, because B_4 was just freed")
        DELAY(7000)
    ENDIF
    LATCH(VON_PROG)
    CLOSE(W_3)
    ACTIVATEL(SIG_SP_3)
    DELAYRANDOM(1500, 2500)
    REV(0)
    DELAY(500)
    // Licht an
    FON(0)
    DELAYRANDOM(500, 1000)
    REV(60)
    START_SEND(B_4_VON_PROG)
    DELAY(5000)
    DEACTIVATEL(SIG_SP_3)
    // todo: check of after is working here, may use AT(-1 * VON_PROG) instead
    AFTER(VON_PROG)
    DELAY(1000)
    PRINT("Loco left prog track")
    ROUTE_INACTIVE(989)
DONE
    
SEQUENCE(B_4_VON_PROG)
    AT(GBM_G4)
    PRINT("B_4_VON_PROG: At G4")
    DELAY(500)
    IFRESERVE(B_4)
        PRINT("B_4_VON_PROG: Reserved B_4")
    ENDIF
    DELAYRANDOM(2500, 3000)
    STOP
    DELAYRANDOM(1000, 2000)
    PRINT("B_4_VON_PROG: Unlatching VON_PROG")
    UNLATCH(VON_PROG)
    DELAYRANDOM(1000, 2500)
    RESERVE_NOESTOP(B_2)
    PRINT("B_4_VON_PROG: Reserved B_2")
    THROW(W_3)
    DELAYRANDOM(1000, 2500)
    FWD(70)
    FOLLOW(WAIT_AT_B_2)
    //IF(GBM_G4)
    //    AFTER(GBM_G4)
    //ENDIF
    //FREE(B_4)
    //PRINT("B_4: Freed B_4")
DONE

SEQUENCE(B_4)
    AT(GBM_G4)
    PRINT("B_4: At G4")
    IF(FREE_B_7)
        FREE(B_7)
        UNLATCH(FREE_B_7)
        PRINT("B_4: Freed B_7")
    ENDIF
    IF(FREE_B_8)
        FREE(B_8)
        UNLATCH(FREE_B_8)
        PRINT("B_4: Freed B_8")
    ENDIF
    DELAY(500)
    IFRESERVE(B_4)
        PRINT("B_4: Reserved B_4")
    ENDIF
    IFRESERVE(B_2)
        PRINT("B_4: Reserved B_2")
    ELSE
        PRINT("B_4: Waiting to reserve B_2")        
        // ONSENSOR(GBM_G4) should did this already
        RESET(ABC_G4)
        DELAY(5000)
        RESERVE_NOESTOP(B_2)
        PRINT("B_4: Reserved B_2 after wait")        
    ENDIF
    IFCLOSED(W_3)
        THROW(W_3)
        DELAY(3000)
    ENDIF
    SET(ABC_G4)
    DELAY(2000)
    FOLLOW(B_2)
    //IF(GBM_G4)
    //    AFTER(GBM_G4)
    //ENDIF
    //FREE(B_4)
    //PRINT("B_4: Freed B_4")
DONE

SEQUENCE(WAIT_AT_B_2)
    AT(GBM_G2)
    PRINT("WAIT_AT_B_2: At G2")
    // try on sensor instead
    // FREE(B_4)
    // PRINT("WAIT_AT_B_2: Freed B_4")
    IFRESERVE(B_2)
        PRINT("WAIT_AT_B_2: Reserved B_2")
    ENDIF
    PRINT("WAIT_AT_B_2: Loco was driven from programming track. Waiting...")
    RESET(ABC_G2)
    DELAYRANDOM(3000, 5000)
    STOP
    FOFF(0)
    SET(ABC_G2)
    ROUTE_INACTIVE(B_2_ROUND)
    DELAY(3000)
    //ROUTE_ACTIVE(B_2_ROUND)
    //FOLLOW(B_2_ROUND)
DONE

SEQUENCE(B_2)
    AT(GBM_G2)
    PRINT("B_2: At G2")
    //FREE(B_4)
    //PRINT("B_2: Freed B_4")
    //SAVESPEED
    IFRESERVE(B_2)
        PRINT("B_2: Reserved B_2")
    ENDIF
    //SLOWDOWN_REL(50)
    //DELAYRANDOM(500, 1500)
    IF(GBM_G1)
        PRINT("B_2: GBM_G1 occupied, waiting...")
        // todo: move to GBM_G1 ONSENSOR
        IF(GBM_G10)
            DEACTIVATEL(SIG_HS_1)
        ELSE
            DEACTIVATEL(SIG_SP_1)
        ENDIF
        RESET(ABC_G2)
        DELAYRANDOM(5000, 20000)
    ELSE
        IF(GBM_G10)
            DEACTIVATEL(SIG_HS_1)
            RESET(ABC_G2)
            DELAYRANDOM(5000, 20000)
        ELSE
            DEACTIVATEL(SIG_SP_1)
            IFRANDOM(70)
                RESET(ABC_G2)
                DELAYRANDOM(10000, 30000)
            ELSE
                IFTHROWN(W_1)
                    RESET(ABC_G2)
                    DELAYRANDOM(5000, 10000)
                    CLOSE(W_1)
                    DELAYRANDOM(2000, 4000)
                ENDIF
            ENDIF
        ENDIF
    ENDIF
    IFRESERVE(B_1)
        PRINT("B_2: Reserved B_1")
    ELSE
        PRINT("B_2: Waiting to reserve B_1")        
        RESET(ABC_G2)
        DELAY(5000)
        RESERVE_NOESTOP(B_1)
        PRINT("B_2: Reserved B_1 after wait")
    ENDIF
    // GBM_G10 könnte noch belegt sein, oder?
    PRINT("B_2: Reserved B_1")
    IFTHROWN(W_1)
        CLOSE(W_1)
        DELAY(2000)
    ENDIF
    CALL(SIG_HS_SP_1_TEST)
    DELAYRANDOM(3000, 5000)
    SET(ABC_G2)
    DELAY(1000)
    //RESTORESPEED
    //IF(GBM_G2)
    //    AFTER(GBM_G2)
    //ENDIF
    //FREE(B_2)
    //PRINT("Freed B_2 by B_2")
    FOLLOW(B_1_VON_BHF)
DONE

AUTOMATION(B_2_ROUND, "B_2 round trip")
    PRINT("B_2_ROUND: Do a round trip")
    ROUTE_DISABLED(B_2_ROUND)
    RESERVE_NOESTOP(B_1)
    PRINT("B_2_ROUND: Reserved B_1")
    IFTHROWN(W_1)
        CLOSE(W_1)
        DELAY(2000)
    ENDIF
    CALL(SIG_HS_SP_1_TEST)
    DELAYRANDOM(3000, 5000)
    FON(0)
    DELAYRANDOM(3000, 5000)
    SET(ABC_G2)
    FWD(70)
    ROUTE_INACTIVE(B_2_ROUND)
    /*
    IF(GBM_G2)
        AFTER(GBM_G2)
    ENDIF
    FREE(B_2)
    PRINT("Freed B_2 by B_2")
    */
    FOLLOW(B_1_VON_BHF)
DONE

AUTOMATION(B_2_ZU_B_3, "B_2 to B_3")
    PRINT("B_2_ZU_B_3: Do a trip to B_3")
    ROUTE_DISABLED(B_2_ZU_B_3)
    RESERVE_NOESTOP(B_1)
    CALL(SIG_HS_SP_1_TEST)
    PRINT("B_2_ZU_B_3: Reserved B_1")
    CLOSE(W_1)
    FON(0)
    DELAYRANDOM(1000, 2000)
    SET(ABC_G2)
    FWD(70)
    AT(GBM_G1)
    PRINT("At G1 from Bhf")
    //FREE(B_2)
    //PRINT("Freed B_2 by B_2_ZU_B_3")
    IFRESERVE(B_1)
        PRINT("Reserved B_1 by B_2_ZU_B_3")
    ENDIF
    DELAY(500)
    STOP
    RESERVE_NOESTOP(B_3)
    PRINT("Reserved B_3 by B_2_ZU_B_3")
    THROW(W_1)
    THROW(W_2)
    DELAYRANDOM(500, 2000)
    REV(70)
    AT(GBM_G3)
    PRINT("B_2_ZU_B_3: At G3")
    DELAY(500)
    //FREE(B_1)
    //PRINT("B_2_ZU_B_3: Freed B_1")
    IFRESERVE(B_3)
        PRINT("B_2_ZU_B_3: Reserved B_3")
    ENDIF
    PRINT("B_2_ZU_B_3: Loco was driven B_2_ZU_B_3. Waiting...")
    RESET(ABC_G3)
    DELAYRANDOM(3000, 5000)
    STOP
    FOFF(0)
    SET(ABC_G3)
    ROUTE_INACTIVE(B_2_ZU_B_3)
DONE

AUTOMATION(B_3_ZU_B_2, "B_3 to B_2")
    PRINT("B_3_ZU_B_2: Do a trip")
    ROUTE_DISABLED(B_3_ZU_B_2)
    RESERVE_NOESTOP(B_1)
    PRINT("B_3_ZU_B_2: Reserved B_1")
    THROW(W_2)
    THROW(W_1)
    FON(0)
    DELAYRANDOM(500, 2000)
    FWD(70)
    ROUTE_INACTIVE(B_3_ZU_B_2)
    /*
    IF(GBM_G2)
        AFTER(GBM_G2)
    ENDIF
    FREE(B_2)
    PRINT("Freed B_2 by B_2")
    */
    FOLLOW(B_1_VON_BHF)
DONE

SEQUENCE(B_1_VON_BHF)
    AT(GBM_G1)
    PRINT("At G1 from Bhf")
    //FREE(B_2)
    //PRINT("Freed B_2 by B_1")
    IFRESERVE(B_1)
        PRINT("Reserved B_1 by B_1_FROM_BHF")
    ENDIF
    DELAY(1000)
    IFRESERVE(B_11)
        PRINT("Reserved B_11 by B_1_FROM_BHF")
    ELSE
        PRINT("B_1_FROM_BHF: Waiting to reserve B_11")        
        RESET(ABC_G9)
        DELAY(5000)
        RESERVE_NOESTOP(B_11)
        PRINT("Reserved B_11 by B_1_FROM_BHF after wait")
    ENDIF
    IFRESERVE(B_8)
        PRINT("Reserved B_8 by B_1_FROM_BHF")
        FOLLOW(B_8)
    ELSE
        IFRESERVE(B_7)
            PRINT("Reserved B_7 by B_1_FROM_BHF")
            FOLLOW(B_7)
        ELSE
            RESET(ABC_G9)
            DELAY(7000)
            IFRESERVE(B_7)
                PRINT("Reserved B_7 by B_1_FROM_BHF")
                FOLLOW(B_7)
            ELSE
                RESERVE_NOESTOP(B_8)
                PRINT("Reserved B_8 by B_1_FROM_BHF")
                FOLLOW(B_8)
            ENDIF
        ENDIF
    ENDIF
DONE

SEQUENCE(B_8)
    PRINT("B_1_VON_BHF: Driving to B_8")
    THROW(W_12)
    THROW(W_11)
    SET(ABC_G9)
    //IF(GBM_G1)
    //    AFTER(GBM_G1)
    //ENDIF
    AT(GBM_G8)
    PRINT("At G8")
    IFRESERVE(B_8)
        PRINT("Reserved B_8 by B_8")
    ENDIF
    //FREE(B_1)
    //PRINT("Freed B_1 by B_8")
    DELAY(1000)
    IFRANDOM(50)
        RESET(ABC_G8)
        IFRESERVE(B_8)
            PRINT("Reserved B_8 by B_8 security")
        ENDIF
        DELAYRANDOM( 5000, 30000)
    ENDIF
    IFRESERVE(B_4)
        PRINT("Reserved B_4 by B_8")
    ELSE
        PRINT("B_8: Waiting to reserve B_4")        
        RESET(ABC_G8)
        DELAY(5000)
        RESERVE_NOESTOP(B_4)
        PRINT("Reserved B_4 by B_8 after wait")
    ENDIF
    PRINT("Reserved B_4 by B_8")
    THROW(W_4)
    DELAY(1000)
    SET(ABC_G8)
    LATCH(FREE_B_8)
    /*
    IF(GBM_G8)
        AFTER(GBM_G8)
    ENDIF
    FREE(B_8)
    PRINT("Freed B_8 by B_8")
    */
    DELAY(1000)
    FOLLOW(B_4)
DONE

SEQUENCE(B_7)
    PRINT("B_1_VON_BHF: Driving to B_7")
    THROW(W_12)
    CLOSE(W_11)
    CLOSE(W_10)
    SET(ABC_G9)
    //IF(GBM_G1)
    //    AFTER(GBM_G1)
    //ENDIF
    AT(GBM_G7)
    PRINT("At G7")
    IFRESERVE(B_7)
        PRINT("Reserved B_7 by B_7")
    ENDIF
    //FREE(B_1)
    //PRINT("Freed B_1 by B_7")
    IFRANDOM(50)
        RESET(ABC_G7)
        IFRESERVE(B_7)
            PRINT("Reserved B_7 by B_7 security")
        ENDIF
        DELAYRANDOM( 5000, 30000)
    ENDIF
    IFRESERVE(B_4)
        PRINT("Reserved B_4 by B_7")
    ELSE
        PRINT("B_7: Waiting to reserve B_4")        
        RESET(ABC_G7)
        DELAY(5000)
        RESERVE_NOESTOP(B_4)
        PRINT("Reserved B_4 by B_7 after wait")
    ENDIF
    PRINT("Reserved B_4 by B_7")
    CLOSE(W_4)
    DELAY(1000)
    SET(ABC_G7)
    LATCH(FREE_B_7)
    /*
    IF(GBM_G7)
        AFTER(GBM_G7)
    ENDIF
    FREE(B_7)
    PRINT("Freed B_7 by B_7")
    */
    DELAY(1000)
    FOLLOW(B_4)
DONE

AUTOMATION(B_3_ROUND, "B_3 round trip")
    PRINT("B_3_ROUND: Do a round trip")
    ROUTE_DISABLED(B_3_ROUND)
    RESERVE_NOESTOP(B_5)
    PRINT("B_3_ROUND: Reserved B_5")
    ACTIVATEL(SIG_HS_5)
    DELAYRANDOM(3000, 5000)
    FON(0)
    DELAYRANDOM(3000, 5000)
    SET(ABC_G3)
    FWD(70)
    ROUTE_INACTIVE(B_3_ROUND)
    FOLLOW(B_5)
DONE

SEQUENCE(B_5)
    AT(GBM_G5)
    PRINT("At G5 from B_3")
    IFRESERVE(B_5)
        PRINT("Reserved B_5 by B_5")
    ENDIF
    IFRESERVE(B_6)
        PRINT("Reserved B_6 by B_5")
    ELSE
        PRINT("B_5: Waiting to reserve B_6")        
        RESET(ABC_G5)
        DELAY(5000)
        RESERVE_NOESTOP(B_6)
        DELAYRANDOM(3000,5000)
        PRINT("Reserved B_6 by B_5 after wait")
    ENDIF
    SET(ABC_G5)
    DELAY(2000)
    FOLLOW(B_6)
DONE

SEQUENCE(B_6)
    AT(GBM_G6)
    PRINT("At G6 from B_5")

    IFRESERVE(B_6)
        PRINT("Reserved B_6 by B_6")
    ENDIF
    DELAY(1000)
    IFRESERVE(B_1)
        PRINT("Reserved B_1 by B_6")
    ELSE
        PRINT("B_6: Waiting to reserve B_1")        
        RESET(ABC_G6)
        DELAY(5000)
        RESERVE_NOESTOP(B_1)
        PRINT("Reserved B_1 by B_6 after wait")
    ENDIF
    IFRESERVE(B_11)
        PRINT("Reserved B_11 by B_6")
    ELSE
        PRINT("B_6: Waiting to reserve B_11")        
        RESET(ABC_G6)
        DELAY(5000)
        RESERVE_NOESTOP(B_11)
        PRINT("Reserved B_11 by B_6 after wait")
    ENDIF
    IFCLOSED(W_10)
        RESET(ABC_G6)
        DELAY(1000)
        THROW(W_10)
    ENDIF
    IFTHROWN(W_11)
        RESET(ABC_G6)
        DELAY(1000)
        CLOSE(W_11)
    ENDIF
    IFCLOSED(W_12)
        RESET(ABC_G6)
        DELAY(1000)
        THROW(W_12)
    ENDIF
    DELAY(1000)
    SET(ABC_G6)
    FOLLOW(B_1)
DONE

SEQUENCE(B_1)
    AT(GBM_G1)
    PRINT("At G1 from B_6")
    IFRESERVE(B_1)
        PRINT("Reserved B_1 by B_1")
    ENDIF
    DELAY(1000)
    IFRESERVE(B_3)
        PRINT("Reserved B_3 by B_1")
    ELSE
        PRINT("B_1: Waiting to reserve B_3")        
        RESET(ABC_G1)
        DELAY(5000)
        RESERVE_NOESTOP(B_3)
        PRINT("Reserved B_3 by B_1 after wait")
        DELAYRANDOM(1000, 3000)
    ENDIF
    IFCLOSED(W_1)
        RESET(ABC_G1)
        DELAY(2000)
        THROW(W_1)
    ENDIF
    IFCLOSED(W_2)
        RESET(ABC_G1)
        DELAY(2000)
        THROW(W_2)
    ENDIF
    SET(ABC_G1)
    DELAY(1000)
    FOLLOW(B_3)
DONE

SEQUENCE(B_3)
    AT(GBM_G3)
    PRINT("At G3 from B_1")
    IFRESERVE(B_3)
        PRINT("Reserved B_3 by B_3")
    ENDIF
    IFRANDOM(70)
        DEACTIVATEL(SIG_HS_5)
        RESET(ABC_G3)
        DELAYRANDOM(10000, 30000)
        IFNOT(GBM_G5)
            ACTIVATEL(SIG_HS_5)
        ENDIF
    ENDIF
    IFRESERVE(B_5)
        PRINT("Reserved B_5 by B_3")
    ELSE
        PRINT("B_3: Waiting to reserve B_5")        
        RESET(ABC_G3)
        DELAY(5000)
        RESERVE_NOESTOP(B_5)
        PRINT("Reserved B_5 by B_3 after wait")
    ENDIF
    DELAY(1000)
    SET(ABC_G3)
    FOLLOW(B_5)
DONE
