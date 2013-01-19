module HaskellText (main) where

import Control.Applicative
import Control.Monad
import Data.ByteString.Lazy.Char8 (ByteString)
import qualified Data.ByteString.Lazy.Char8 as B
import Data.Char
import Data.Int
import Data.Vector.Unboxed (Vector, (!), (//))
import qualified Data.Vector.Unboxed as V
import Data.Word
import System.IO

-- | A block requiring the reverse complement.  The first element is the
-- header string. The second element is the actual data.
data Block = Block ByteString ByteString

main :: IO ()
main = getBlocks >>= mapM_ (writeBlock . reverseComplement)

-- | Write a block to stdio with line breaks.
writeBlock :: Block -> IO ()
writeBlock (Block name txt) = do
      putChar '>'
      B.putStrLn name
      mapM_ B.putStrLn $ splitEvery 60 txt


-- | A utility function that ought to be part of the bytestring library.
splitEvery :: Int64 -> ByteString -> [ByteString]
splitEvery n t = go t
  where
    go t = if (not $ B.null t2) then t1 : go t2 else [t1]
      where (t1, t2) = B.splitAt n t


-- | Read a series of blocks, each of which starts with a '>' character.
getBlocks :: IO [Block]
getBlocks = map mkBlock . drop 1 . B.split '>' <$> B.getContents
   where 
      mkBlock bs1 = Block h t
        where (h, t) = B.break (== '\n') bs1
  

reverseComplement :: Block -> Block
reverseComplement (Block name codes) = 
   Block name
   $ B.filter (/= '\0')
   $ B.map (V.unsafeIndex tbl . ord)
   $ B.reverse codes
   where
      tbl = V.replicate 256 '\0' //
            map (first ord) (pairs ++ map (first toLower) pairs)
      pairs = [('A', 'T'),('C', 'G'),('G', 'C'),('T', 'A'),
               ('M', 'K'),('R', 'Y'),('W', 'W'),('S', 'S'),
               ('Y', 'R'),('K', 'M'),('V', 'B'),('H', 'D'),
               ('D', 'H'),('B', 'V'),('N', 'N')]
      first f (x,y) = (f x, y)
