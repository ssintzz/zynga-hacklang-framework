<?hh //strict

namespace Zynga\Framework\Service\V2\Client\Mock;

use Zynga\Framework\Service\V2\Client\Mock\Batch as MockBatch;
use Zynga\Framework\Service\V2\Client\Mock\HasResponseBatch as Batch;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class HasResponseBatchTest extends TestCase {
  public function testIsBatchStarted(): void {
    $batch = new Batch(new MockBatch());
    $this->assertFalse($batch->isBatchStarted());
  }
}
