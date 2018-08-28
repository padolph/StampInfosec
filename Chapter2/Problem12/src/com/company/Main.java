package com.company;

import java.io.*;
import java.util.*;

import static java.util.stream.Collectors.toMap;

public class Main {

    // "A date which will live in infamy" speech, FDR
    // key = CHIKOBTRLXAPYMGWSFEQNVJUxD
    private static String ciphertext =
            "GBSXUCGSZQGKGSQPKQKGLSKASPCGBGBKGUKGCEUKUZKGGBSQEICA" +
            "CGKGCEUERWKLKUPKQQGCIICUAEUVSHQKGCEUPCGBCGQOEVSHUNSU" +
            "GKUZCGQSNLSHEHIEEDCUOGEPKHZGBSNKCUGSUKUASERLSKASCUGB" +
            "SLKACRCACUZSSZEUSBEXHKRGSHWKLKUSQSKCHQTXKZHEUQBKZAEN" +
            "NSUASZFENFCUOCUEKBXGBSWKLKUSQSKNFKQQKZEHGEGBSXUCGSZQ" +
            "GKGSQKUZBCQAEIISKOXSZSICVSHSZGEGBSQSAHSGKHMERQGKGSKR" +
            "EHNKIHSLIMGEKHSASUGKNSHCAKUNSQQKOSPBCISGBCQHSLIMQGKG" +
            "SZGBKGCGQSSNSZXQSISQQGEAEUGCUXSGBSSJCQGCUOZCLIENKGCA" +
            "USOEGCKGCEUQCGAEUGKCUSZUEGBHSKGEHBCUGERPKHEHKHNSZKGGKAD";

    // "The Walrus and the Carpenter", Lewis Carroll
    // key = DHPUWEBRYLKGxXFTAxxOVINSMC
//    private static String ciphertext =
//            "PBFPVYFBQXZTYFPBFEQJHDXXQVAPTPQJKTOYQWIPBVWLXTOXBTFXQWAXBVCXQWAXFQJVWLEQNTOZQGGQLFXQWAKVWLXQWAEBIPBFXFQV" +
//            "XGTVJVWLBTPQWAEBFPBFHCVLXBQUFEVWLXGDPEQVPQGVPPBFTIXPFHXZHVFAGFOTHFEFBQUFTDHZBQPOTHXTYFTODXQHFTDPTOGHFQPB" +
//            "QWAQJJTODXQHFOQPWTBDHHIXQVAPBFZQHCFWPFHPBFIPBQWKFABVYYDZBOTHPBQPQJTQOTOGHFQAPBFEQJHDXXQVAVXEBQPEFZBVFOJI" +
//            "WFFACFCCFHQWAUVWFLQHGFXVAFXQHFUFHILTTAVWAFFAWTEVOITDHFHFQAITIXPFHXAFQHEFZQWGFLVWPTOFFA";

    public static void main(String[] args) {

        final Map<Character, Integer> ciphertextLetterFrequencies = getLetterFrequencies(ciphertext);
        printLetterFrequencies(ciphertextLetterFrequencies);

        final char[] lettersSortedByCiphertextFrequency = getLettersSortedByCiphertextFrequency(ciphertextLetterFrequencies);

        final List<String> words = readDictionary();

        char[] key = computeBestKey(lettersSortedByCiphertextFrequency, words);

        String plaintext = decode(ciphertext, key);
        int score = computeScore(plaintext, words);

        System.out.println(ciphertext);
        System.out.println(decode(ciphertext, key));
        System.out.println("Score " + score);
        System.out.println("Candidate Key: ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        System.out.println("               " + new String(key));

        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("\n               ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            System.out.print("Enter key:     ");
            String keyStr = scanner.next();
            if (keyStr.length() != 26) break;
            System.out.println(ciphertext);
            plaintext = decode(ciphertext, keyStr.toCharArray());
            System.out.println(plaintext);
            score = computeScore(plaintext, words);
            System.out.println("Score " + score);
        }

    }

    private static char[] computeBestKey(char[] lettersSortedByCiphertextFrequency, List<String> words) {
        char[] lettersSortedByReferenceFrequency = {'E','T','A','O','I','N','S','H','R','D','L','C','U','M','W','F','G','Y','P','B','V','K','J','X','Q','Z'};

        // initial guess at key
        char[] key = buildKey(lettersSortedByCiphertextFrequency, lettersSortedByReferenceFrequency);
        String plaintext = decode(ciphertext, key);
        int bestScore = computeScore(plaintext, words);

        // mess with key to try to get a better score, stop when no improvement found
        char [] finalKey = new char[26];
        int lastScore;
        int count = 0;
        do {
            lastScore = bestScore;
            for (int i = 0; i < 25; i++) {
                char[] candidateLettersSortedByReferenceFrequency = swapAdjacentLetterFrequencies(lettersSortedByReferenceFrequency, i);
                char[] candidateKey = buildKey(lettersSortedByCiphertextFrequency, candidateLettersSortedByReferenceFrequency);
                plaintext = decode(ciphertext, candidateKey);
                int score = computeScore(plaintext, words);
                if (score > bestScore) {
                    lettersSortedByReferenceFrequency = candidateLettersSortedByReferenceFrequency;
                    finalKey = candidateKey;
                    bestScore = score;
                }
            }
            if (bestScore == lastScore) {
                count++;
            }
        } while (count < 2);

        return finalKey;
    }

    private static char[] buildKey(char[] lettersSortedByCiphertextFrequency, char[] lettersSortedByReferenceFrequency) {
        char[] key = new char[26];
        for (int i = 0; i < 26; i++) {
            int keyIdx = lettersSortedByCiphertextFrequency[i] - 65;
            if (keyIdx >= 0) {
                key[keyIdx] = lettersSortedByReferenceFrequency[i];
            }
        }
        for (int i = 0; i < 26; i++) {
            if (key[i] < 65) {
                key[i] = 'x';
            }
        }
        return key;
    }

    private static final char[] swapAdjacentLetterFrequencies(char[] letterFrequencies, int position) {
        char[] newLetterFrequencies = new char[26];
        System.arraycopy( letterFrequencies, 0, newLetterFrequencies, 0, 26 );;
        char tmp = newLetterFrequencies[position];
        newLetterFrequencies[position] = newLetterFrequencies[position + 1];
        newLetterFrequencies[position + 1] = tmp;
        return newLetterFrequencies;
    }

    private static char[] getLettersSortedByCiphertextFrequency(Map<Character, Integer> letterFrequencies) {
        Map<Character, Integer> sortedLetterFrequencies = letterFrequencies
                .entrySet()
                .stream()
                .sorted(Collections.reverseOrder(Map.Entry.comparingByValue()))
                .collect(toMap(Map.Entry::getKey, Map.Entry::getValue, (e1, e2) -> e2, LinkedHashMap::new));
        char[] lettersSortedByCiphertextFrequency = new char[26];
        int i = 0;
        Iterator it = sortedLetterFrequencies.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry pair = (Map.Entry)it.next();
            char character = (char)(pair.getKey());
            lettersSortedByCiphertextFrequency[i++] = character;
        }
        return lettersSortedByCiphertextFrequency;
    }

    private static int computeScore(String text, List<String> words) {
        int score = 0;
        for (String word : words) {
            int lastIndex = 0;
            while(lastIndex != -1){
                lastIndex = text.indexOf(word, lastIndex);
                if(lastIndex != -1){
                    score++;
                    lastIndex += word.length();
                }
            }
        }
        return score;
    }

    private static List<String> readDictionary() {
        InputStream is = new Main().getClass().getResourceAsStream("words_alpha.txt");
        List<String> words = new ArrayList<>();
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(is));
            String word;
            if (is != null) {
                while ((word = reader.readLine()) != null) {
                    words.add(word.toUpperCase());
                }
            }
        } catch (IOException e) {
            System.out.println("IOException");
        } finally {
            try { is.close(); } catch (Throwable ignore) {}
        }
        return words;
    }

    private static void printLetterFrequencies(Map<Character, Integer> map) {
        Character[] charactersFound = map.keySet().toArray(new Character[0]);
        System.out.println("Letter\tFrequency\tCount");
        char character;
        float totalCount = 0.0f;
        for (int f : map.values()) {
            totalCount += (float)f;
        }
        for (int i = 0; i < charactersFound.length; i++)
        {
            character = charactersFound[i];
            System.out.format("%s       %2.2f%10d\n", character, map.get(character)/totalCount*100, map.get(character));
        }
    }

    private static Map<Character, Integer> getLetterFrequencies(String input) {
        Map<Character, Integer> map = new HashMap<Character,Integer>();
        for (int i = 0; i < input.length(); i++)
        {
            char character = input.charAt(i);
            Integer countForCharacter = 0;

            if (map.containsKey(character)) {
                countForCharacter = map.get(character);
                countForCharacter++;
            } else {
                countForCharacter = 1;
            }
            map.put(character, countForCharacter);
        }
        return map;
    }

    private static String decode(String ciphertext, char[] key) {
        assert (key.length == 26);
        byte[] textBytes = Main.ciphertext.getBytes();
        StringBuilder stringBuilder = new StringBuilder();
        for (byte character : textBytes) {
            int keyIdx = character - 65; // 'A' = 0
            assert(keyIdx >= 0 && keyIdx <= 26);
            char plaintextChar = key[keyIdx];
            stringBuilder.append(plaintextChar);
        }
        return stringBuilder.toString();
    }

}
